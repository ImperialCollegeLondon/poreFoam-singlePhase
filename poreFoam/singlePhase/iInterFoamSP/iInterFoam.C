/*---------------------------------------------------------------------------*\

\*---------------------------------------------------------------------------*/
#define SINGLE_PHASE
#define ifMonitor  if( runTime.timeIndex()%5== 0 ) 

#include "fvCFD.H"


//#include "singlePhaseTransportModel.H"
//#include "turbulentTransportModel.H"


#include "fixedFluxPressureFvPatchScalarField.H"

#include "pimpleControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //



int main(int argc, char *argv[])
{ 
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    if (!mesh.cells().size()) {Info<<"Error: no cells in (processor) mesh"<<endl; exit(-1);}
    pimpleControl pimple(mesh);
    #include "initContinuityErrs.H"
    #include "createFields.H"
    #include "createTimeControls.H"
    #include "correctPhi.H"
    #include "CourantNo.H"
    
    //~ #include "setInitialDeltaT.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;
#define 	curtail(a,b) (min (max((a),(b)),(1.-(b))))

	ifMonitor
	{
		Info<< "\n         Umax = " << max(mag(U)).value() << " m/s  "
		<< "Uavg = " << mag(average(U)).value() << " m/s"
		<< "   DP = " << (max(p)-min(p)).value() << " Pa"
		<< nl<< nl << endl;
	}
		
	Info <<"min(p): "<<min(p)<<"  max(p): "<<max(p)<<endl;



	scalar pRelaxF=0.1;





    while (runTime.run())
    {
        scalar timeStepExecutionTime= runTime.elapsedCpuTime() ;
        //~ #include "readPISOControls.H"
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;


    
        dimensionedScalar sgPc("sgPc", dimPressure/dimLength*dimArea, 0.0);
        while (pimple.loop())
        {
		 //~ rhoPhi = rho1*phi;


				{
					volScalarField pOldCopy=p; 
					p=0.33*pOldOld+0.33*pOld+0.34*p;
					pOldOld=pOld;
					pOld=pOldCopy;
				}

				scalar tOldPU= runTime.elapsedCpuTime() ;



                #include "UEqn.H"

                ifMonitor   { Info<< "ExeTime - tOldPU = " << runTime.elapsedCpuTime()-tOldPU << " s"	<< endl;}

                // --- PISO loop
                while (pimple.correct())
                {
                    #include "pEqn.H"
                }




    
      
        }           

        #include "continuityErrs.H"
		

			
			
			
			
			Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
			<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
			<< "  timeStepExecutionTime  = " << runTime.elapsedCpuTime()-timeStepExecutionTime << " s"
			<< endl;
			
			ifMonitor
			{
				scalar maxU = max(mag(U)).value();
				scalar avgU = average(mag(U)).value();

				Info<< "\n         maxMagU = " <<maxU << " m/s  "
				<< "avgMagU = " << avgU << " m/s"
				<< "   DP = " << (max(p)-min(p)).value() << " Pa"
				<< nl<< nl << endl;
				
				scalar delAvgUPer10Step =  mag(avgU - oldAvgU);
				if (delAvgUPer10Step<thresholdDelUPer10Step*max(avgU,oldAvgU) && oldelAvgUPer10Step<thresholdDelUPer10Step*max(avgU,oldAvgU))
				{
					Info<< "converged ! " 	<< endl;
					
					runTime.writeNow();
					return 0;
				}
				oldelAvgUPer10Step=delAvgUPer10Step;
				oldAvgU=avgU;
			}




            scalar maxExecutionTime
            (
                readScalar(runTime.controlDict().lookup("maxExecutionTime"))
            );				
		    #include "write.H"
		    if (runTime.elapsedClockTime()>maxExecutionTime)
		    {
				Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
					<< "  ClockTime = " << runTime.elapsedClockTime() << " s\n"
					<< maxExecutionTime/24/60/60<<"  days passed!, ending simulation. " 
					<< endl;
				
				runTime.writeNow();
				return 0;
			}
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
