
	volScalarField rAU = 1.0/UEqn().A();
	surfaceScalarField rAUf = fvc::interpolate(rAU);

	U = rAU*(UEqn().H());
  
	if (pimple.nCorrPISO() <= 1)
	{
		UEqn.clear();
	}

phi = (fvc::interpolate(U) & mesh.Sf())
	  + rho*fvc::interpolate(rAU)*fvc::ddtCorr(U, phi);

adjustPhi(phi, U, p);


	// Update the fixedFluxPressure BCs to ensure flux consistency
	setSnGrad<fixedFluxPressureFvPatchScalarField> ( p.boundaryFieldRef(),
		( phi.boundaryField()  // - fvOptions.relative(mesh.Sf().boundaryField() & U.boundaryField())
		) / (mesh.magSf().boundaryField()*rAUf.boundaryField())	);

	for(int nonOrth=0; nonOrth<=pimple.nNonOrthCorr(); nonOrth++)
	{
		fvScalarMatrix pEqn
		(
			fvm::laplacian(rAUf, p) == fvc::div(phi)
		);

		pEqn.setReference(pRefCell, pRefValue);


		pEqn.solve
		(
			mesh.solver(p.select(pimple.finalInnerIter()))
		);

		if (pimple.finalNonOrthogonalIter())
			phi -= pEqn.flux();
	}


	U -= rAU*(fvc::grad(p));	//
	//~ U += rAU*fvc::reconstruct((phi - phiU)/rAUf);
	U.correctBoundaryConditions();

