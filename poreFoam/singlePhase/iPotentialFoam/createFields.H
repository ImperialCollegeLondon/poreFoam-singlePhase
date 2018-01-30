
const surfaceVectorField & 	Sf = mesh.Sf();
const_cast<pointField*>(&mesh.points())->resize(0);
const_cast<faceList*>(&mesh.faces())->resize(0);

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (	IOobject
		(
			"U",  runTime.timeName(),  mesh,
			IOobject::MUST_READ,  IOobject::AUTO_WRITE
		),  mesh
    );
    
    Info<< "Reading field p\n" << endl;
    volScalarField p
    (  IOobject
        (  "p",  runTime.timeName(),  mesh,
            IOobject::MUST_READ,  IOobject::AUTO_WRITE
        ),  mesh
    );


    #include "createPhi.H"



    Info<< "Reading transportProperties\n" << endl;
    IOdictionary transportProperties
    (	IOobject
		( "transportProperties",  runTime.constant(),  runTime,
		IOobject::MUST_READ,  IOobject::NO_WRITE
    )	);
    
	const dimensionedScalar rho(transportProperties.lookup("rho"));
	const dimensionedScalar nu(transportProperties.lookup("nu"));
	const dimensionedScalar mu(rho*nu);
	



    //~ // Construct incompressible turbulence model
    //~ autoPtr<incompressible::turbulenceModel> turbulence
    //~ (
        //~ incompressible::turbulenceModel::New(U, phi, transportProperties)
    //~ );
    

	scalar cBC =readScalar( pimple.dict().lookup("cBC")  );



    label pRefCell = 0;
    scalar pRefValue = 0.0;
	setRefCell(p, pimple.dict(), pRefCell, pRefValue);







	scalar lambda( readScalar(pimple.dict().lookup("lambda")) );



    U.correctBoundaryConditions();


scalar deltaTFact=1.0;

	label nUCorr
	(
		readLabel(pimple.dict().lookup("nUCorr"))
	);
	dimensionedScalar beta ( pimple.dict().lookup("beta")  );
    
