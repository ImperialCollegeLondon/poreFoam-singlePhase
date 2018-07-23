
vector FF(0.0, 0.0, 0.0);
{
	Info<< "Reading field p\n" << endl;

	//volScalarField& V=p;//("V",p);

	Info<< "Reading transportProperties\n" << endl;



	Info<< "Reading diffusivity K\n" << endl;



	//dimensionedScalar fFact = transportProperties.lookupOrDefault("fF",dimensionedScalar("fF", dimless, 1.0));

	volScalarField kEf
	(	IOobject
		(	"kEf",	"0",	mesh,
			IOobject::READ_IF_PRESENT,	IOobject::AUTO_WRITE
		),
		1.0/porosity/porosity,"zeroGradient"
	);
	kEf.correctBoundaryConditions();
	Info<<"avg kEf"<<average(kEf)<<endl;

	pimpleControl pimple(mesh);

	// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	Info<< "\ncalculating formation factor \n" << endl;

	for(int iter = 0; iter<2; ++iter)
	{
 
		//runTime++;

		while (pimple.loop())
		{

			Info<< "Time = " << runTime.timeName() << nl << endl;

			while (pimple.correctNonOrthogonal())
			{
				solve
				(
					fvm::laplacian(kEf, p)
				);
			}

			Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
				<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
				<< nl << endl;
		}
	}
	
	
	phi=fvc::interpolate(kEf*dimensionedScalar("one", dimLength*dimLength, 1.0)/(mu))*fvc::snGrad(p)*mesh.magSf();

	Info<< ".\n" << endl;

	{
		Info<<"Writting (Electrical) potential, psi"<<endl;
		volScalarField psi
		(	IOobject
			(	"psi",	"0",	mesh,
				IOobject::NO_READ,	IOobject::AUTO_WRITE
			),
			p
		);	psi.write();
	}


	{
		vector dV;
		label iLeft = mesh.boundaryMesh().findPatchID(LeftPs[iDir]);
		if (iLeft < 0)	 { Info	<< "Unable to find  patch " << LeftPs[iDir] << nl	<< endl;	}
		label iRight = mesh.boundaryMesh().findPatchID(RightPs[iDir]);
		if (iRight < 0) { Info	<< "Unable to find  patch " << RightPs[iDir] << nl	<< endl;	}
	
		Info<<gSum(kEf.boundaryField()[iRight])<<"  "<<endl;

		if( iLeft>=0 && iRight>=0)
		{

			scalar fluxIn=gSum(phi.boundaryField()[iLeft]);

			scalar PLeft=gSum(p.boundaryField()[iLeft]*(phi.boundaryField()[iLeft]))/(fluxIn+1.0e-29);
			scalar PRight=gSum(p.boundaryField()[iRight]*(phi.boundaryField()[iRight]))/(fluxIn+1.0e-29);
			dV[iDir]=mag(PLeft-PRight);

			FF[iDir]=dV[iDir]*A[iDir]/L[iDir]/(mag(fluxIn)+1.0e-126)/(mu.value());
		}
	}
}
