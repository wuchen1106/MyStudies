

#include "G4Geo/MdcG4Geo.h"
#include "G4Geo/BesG4Geo.h"
#include "KalFitAlg/KalFitAlg.h"
#include "KalFitAlg/KalFitTrack.h"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "GDMLProcessor.hh"

#include "G4NistManager.hh"
#include "MdcSim/BesMdcGemGeo.hh"
#include "math.h"


void KalFitAlg::setBesFromGdml(void){

	int i(0);
	double Z(0.),A(0.),Ionization(0.),Density(0.),Radlen(0.);

	G4LogicalVolume *logicalMdc = 0;
	MdcG4Geo* aMdcG4Geo = new MdcG4Geo();
	logicalMdc = aMdcG4Geo->GetTopVolume();   
	//BesMdcGemGeo* gem=BesMdcGemGeo::GetGeo(useNCGem_);
	int imdc=2;
	switch(useNCGem_) 
	{
		case 0:
			imdc=2;//pure Mdc
			break;
		case 1:
			imdc=5;//4 Mdc, 1 GEM, no CF
			break;
		case 2:
			imdc=4;//4 Mdc Layer, 2 GEM Layer,no Carbon Fiber
			break;
		case 3:
			imdc=11;//0 Mdc Layer, 3 GEM Layer (new),no Carbon Fiber
			break;
		case 4:
			imdc=1;//no Inner Mdc,4 GEM Layer,no Carbon Fiber inner wall
			break;
		case 13:
			imdc=12;//0 Mdc Layer, 3 GEM Layer (new but normal arrangement),no Carbon Fiber
			break;
		case 14:
			imdc=13;//no Inner Mdc,4 GEM Layer (new) ,no Carbon Fiber inner wall
			break;
		defualt:
			imdc=2;
			cout<<"Warning::Unexisted useNCGem_ Option!!!"<<endl;
	}

	cout<<"imdc = "<<imdc<<endl;
	BesMdcGemGeo* gem = new BesMdcGemGeo(imdc);
	//BesMdcGemGeo* gem=BesMdcGemGeo::GetGeo();

	/// mdcgas
	G4Material* mdcMaterial = logicalMdc->GetMaterial();  

	for(i=0; i<mdcMaterial->GetElementVector()->size(); i++){
		Z += (mdcMaterial->GetElement(i)->GetZ())*
			(mdcMaterial->GetFractionVector()[i]);
		A += (mdcMaterial->GetElement(i)->GetA())*
			(mdcMaterial->GetFractionVector()[i]);
	}
	Ionization = mdcMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = mdcMaterial->GetDensity()/(g/cm3);
	Radlen = mdcMaterial->GetRadlen();
	std::cout<<"mdcgas: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitMdcMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.); 
	_BesKalmanFitMaterials.push_back(FitMdcMaterial);// 0
	KalFitTrack::mdcGasRadlen_ = Radlen/10.;

	/// inner wall shield fiml1 Al by wangll 2012-09-07
	G4LogicalVolume* innerWallFilm1Volume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("LogicalMdcInnerFilm1"));
	G4Material* innerWallFilm1Material = innerWallFilm1Volume->GetMaterial();
	G4Tubs* innerwallFilm1Tub = dynamic_cast<G4Tubs*>(innerWallFilm1Volume->GetSolid());

	Z = 0.;
	A = 0.;
	for(i=0; i<innerWallFilm1Material->GetElementVector()->size(); i++){    
		Z += (innerWallFilm1Material->GetElement(i)->GetZ())*
			(innerWallFilm1Material->GetFractionVector()[i]); 
		A += (innerWallFilm1Material->GetElement(i)->GetA())*	
			(innerWallFilm1Material->GetFractionVector()[i]);
	}

	Ionization = innerWallFilm1Material->GetIonisation()->GetMeanExcitationEnergy();
	Density = innerWallFilm1Material->GetDensity()/(g/cm3);
	Radlen = innerWallFilm1Material->GetRadlen();
	std::cout<<"Mdc innerwall Film1, Al: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitInnerwallFilm1Material(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitInnerwallFilm1Material);// 1


	/// inner wall CarbonFiber by wll 2012-09-06
	G4LogicalVolume* innerwallVolume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("logicalMdcSegment2"));
	G4Material* innerwallMaterial = innerwallVolume->GetMaterial();
	G4Tubs* innerwallTub = dynamic_cast<G4Tubs*>(innerwallVolume->GetSolid());

	Z = 0.;
	A = 0.;
	for(i=0; i<innerwallMaterial->GetElementVector()->size(); i++){    
		Z += (innerwallMaterial->GetElement(i)->GetZ())*
			(innerwallMaterial->GetFractionVector()[i]); 
		A += (innerwallMaterial->GetElement(i)->GetA())*	
			(innerwallMaterial->GetFractionVector()[i]);
	}

	Ionization = innerwallMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = innerwallMaterial->GetDensity()/(g/cm3);
	Radlen = innerwallMaterial->GetRadlen();
	std::cout<<"Mdc innerwall, Al: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitInnerwallMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitInnerwallMaterial);// 2

	/// inner wall shield film0 Al by wangll 2012-09-07
	G4LogicalVolume* innerWallFilm0Volume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("LogicalMdcInnerFilm0"));
	G4Material* innerWallFilm0Material = innerWallFilm0Volume->GetMaterial();
	G4Tubs* innerwallFilm0Tub = dynamic_cast<G4Tubs*>(innerWallFilm0Volume->GetSolid());

	Z = 0.;
	A = 0.;
	for(i=0; i<innerWallFilm0Material->GetElementVector()->size(); i++){    
		Z += (innerWallFilm0Material->GetElement(i)->GetZ())*
			(innerWallFilm0Material->GetFractionVector()[i]); 
		A += (innerWallFilm0Material->GetElement(i)->GetA())*	
			(innerWallFilm0Material->GetFractionVector()[i]);
	}

	Ionization = innerWallFilm0Material->GetIonisation()->GetMeanExcitationEnergy();
	Density = innerWallFilm0Material->GetDensity()/(g/cm3);
	Radlen = innerWallFilm0Material->GetRadlen();
	std::cout<<"Mdc innerwall Film0, Al: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitInnerwallFilm0Material(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitInnerwallFilm0Material);// 3

	///////////////////////////////////////////////////////////////////////////////////////////////////  
	G4LogicalVolume *logicalBes = 0;
	BesG4Geo* aBesG4Geo = new BesG4Geo();
	logicalBes = aBesG4Geo->GetTopVolume();

	/// air
	G4LogicalVolume* logicalAirVolume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("logicalWorld"));
	G4Material* airMaterial = logicalAirVolume->GetMaterial();
	Z = 0.;
	A = 0.;
	for(i=0; i<airMaterial->GetElementVector()->size(); i++){
		Z += (airMaterial->GetElement(i)->GetZ())*
			(airMaterial->GetFractionVector()[i]);
		A += (airMaterial->GetElement(i)->GetA())*
			(airMaterial->GetFractionVector()[i]);
	}

	Ionization = airMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = airMaterial->GetDensity()/(g/cm3);
	Radlen = airMaterial->GetRadlen();
	std::cout<<"air: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitAirMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitAirMaterial);// 4

	/// outer beryllium pipe 
	G4LogicalVolume* logicalOuterBeVolume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("logicalouterBe"));
	G4Material* outerBeMaterial = logicalOuterBeVolume->GetMaterial();

	G4Tubs* outerBeTub = dynamic_cast<G4Tubs*>(logicalOuterBeVolume->GetSolid());
	Z = 0.;
	A = 0.;
	for(i=0; i<outerBeMaterial->GetElementVector()->size(); i++){   
		Z += (outerBeMaterial->GetElement(i)->GetZ())*
			(outerBeMaterial->GetFractionVector()[i]);   
		A += (outerBeMaterial->GetElement(i)->GetA())*   
			(outerBeMaterial->GetFractionVector()[i]);
	}
	Ionization =  outerBeMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = outerBeMaterial->GetDensity()/(g/cm3);
	Radlen = outerBeMaterial->GetRadlen();
	std::cout<<"outer beryllium: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitOuterBeMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitOuterBeMaterial);// 5


	/// cooling oil 
	G4LogicalVolume* logicalOilLayerVolume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("logicaloilLayer"));
	G4Material* oilLayerMaterial = logicalOilLayerVolume->GetMaterial();
	G4Tubs* oilLayerTub = dynamic_cast<G4Tubs*>(logicalOilLayerVolume->GetSolid());

	Z = 0.;
	A = 0.;
	for(i=0; i<oilLayerMaterial->GetElementVector()->size(); i++){        
		Z += (oilLayerMaterial->GetElement(i)->GetZ())*
			(oilLayerMaterial->GetFractionVector()[i]);             
		A += (oilLayerMaterial->GetElement(i)->GetA())*             
			(oilLayerMaterial->GetFractionVector()[i]);
	}
	Ionization = oilLayerMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = oilLayerMaterial->GetDensity()/(g/cm3);
	Radlen = oilLayerMaterial->GetRadlen();
	std::cout<<"cooling oil: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitOilLayerMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitOilLayerMaterial);// 6


	/// inner beryllium pipe 
	G4LogicalVolume* logicalInnerBeVolume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("logicalinnerBe"));
	G4Material* innerBeMaterial = logicalInnerBeVolume->GetMaterial();
	G4Tubs* innerBeTub = dynamic_cast<G4Tubs*>(logicalInnerBeVolume->GetSolid());
	Z = 0.;
	A = 0.;
	for(i=0; i<innerBeMaterial->GetElementVector()->size(); i++){
		Z += (innerBeMaterial->GetElement(i)->GetZ())*
			(innerBeMaterial->GetFractionVector()[i]);
		A += (innerBeMaterial->GetElement(i)->GetA())*
			(innerBeMaterial->GetFractionVector()[i]);
	}

	Ionization = innerBeMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = innerBeMaterial->GetDensity()/(g/cm3);
	Radlen = innerBeMaterial->GetRadlen();
	std::cout<<"inner beryllium: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitInnerBeMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitInnerBeMaterial);// 7


	/// gold
	G4LogicalVolume* logicalGoldLayerVolume = const_cast<G4LogicalVolume*>(GDMLProcessor::GetInstance()->GetLogicalVolume("logicalgoldLayer"));
	G4Material* goldLayerMaterial = logicalGoldLayerVolume->GetMaterial();
	G4Tubs* goldLayerTub = dynamic_cast<G4Tubs*>(logicalGoldLayerVolume->GetSolid());

	Z = 0.;
	A = 0.;
	for(i=0; i<goldLayerMaterial->GetElementVector()->size(); i++){
		Z += (goldLayerMaterial->GetElement(i)->GetZ())*
			(goldLayerMaterial->GetFractionVector()[i]);
		A += (goldLayerMaterial->GetElement(i)->GetA())*
			(goldLayerMaterial->GetFractionVector()[i]);
	}
	Ionization = goldLayerMaterial->GetIonisation()->GetMeanExcitationEnergy();
	Density = goldLayerMaterial->GetDensity()/(g/cm3);
	Radlen = goldLayerMaterial->GetRadlen();
	std::cout<<"gold layer: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitGoldLayerMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitGoldLayerMaterial);// 8

// >>>>>>>
	// CDC gas (He:C4H10=90:10) by wuchen 2013-06-23
	Z = 3.88432;
	A = 7.67152*g/mole
	Ionization = 48.6658*eV
	Density = 0.000403321*g/cm3
	Radlen = 1414900*mm
	std::cout<<"mdcgas: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitMdcMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.); 
	_BesKalmanFitMaterials.push_back(FitMdcMaterial);// 0
	KalFitTrack::mdcGasRadlen_ = Radlen/10.;

	/// CDC inner wall shield (Carbon Fiber) by wuchen 2013-06-23
	Z = 6.5633;
	A = 13.1278*g/mole
	Ionization = 83.4461*eV
	Density = 1.57*g/cm3
	Radlen = 253.835*mm
	std::cout<<"Mdc innerwall Film1, Al: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
	KalFitMaterial FitInnerwallFilm1Material(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
	_BesKalmanFitMaterials.push_back(FitInnerwallFilm1Material);// 1
// <<<<<<<


	/// now construct the cylinders
	double radius, thick, length , z0;

	if(useNCGem_>0) {
		/// innerwall of inner drift chamber
		radius = 180.5 *mm/(cm);
		thick  = innerwallTub->GetOuterRadius()/(cm) - innerwallTub->GetInnerRadius()/(cm);
		length = 914 *mm/cm;
		z0     = 0.0;
		std::cout<<"innerwall: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
		KalFitCylinder innerwallCylinder(&_BesKalmanFitMaterials[2], radius, thick, length , z0);
		//m_innerWall.push_back(innerwallCylinder);

		//********************** GEM Tube *********************

		// Lead material defined using NIST Manager
		G4NistManager* nistManager = G4NistManager::Instance();
		nistManager->FindOrBuildMaterial("G4_KAPTON");
		nistManager->FindOrBuildMaterial("G4_Ar");
		nistManager->FindOrBuildMaterial("G4_Cu");
		nistManager->FindOrBuildMaterial("G4_Au");
		nistManager->FindOrBuildMaterial("G4_Al");
		nistManager->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

		double z;
		double a = 1.01*g/mole;
		G4Element* H  = new G4Element("Hydrogen", "H" , z= 1., a);
		a = 12.01*g/mole;
		G4Element* C  = new G4Element("Carbon"  , "C" , z= 6., a);
		a = 14.01*g/mole;
		G4Element* N  = new G4Element("Nitrogen", "N" , z= 7., a);
		a = 16.00*g/mole;
		G4Element* O  = new G4Element("Oxygen"  , "O" , z= 8., a);

		//Cu Foil
		G4Material* Cu = G4Material::GetMaterial("G4_Cu");
		Z = 0.;
		A = 0.;
		for(i=0; i<Cu->GetElementVector()->size(); i++){    
			Z += (Cu->GetElement(i)->GetZ())*
				(Cu->GetFractionVector()[i]); 
			A += (Cu->GetElement(i)->GetA())*	
				(Cu->GetFractionVector()[i]);
		}

		Ionization = Cu->GetIonisation()->GetMeanExcitationEnergy();
		Density = Cu->GetDensity()/(g/cm3);
		Radlen = Cu->GetRadlen();
		std::cout<<"Gem CuFoil, Cu: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitCuMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitCuMaterial);  //0

		//Kapton Foil
		G4Material* Kapton = G4Material::GetMaterial("G4_KAPTON");
		Z = 0.;
		A = 0.;
		for(i=0; i<Kapton->GetElementVector()->size(); i++){    
			Z += (Kapton->GetElement(i)->GetZ())*
				(Kapton->GetFractionVector()[i]); 
			A += (Kapton->GetElement(i)->GetA())*	
				(Kapton->GetFractionVector()[i]);
		}

		Ionization = Kapton->GetIonisation()->GetMeanExcitationEnergy();
		Density = Kapton->GetDensity()/(g/cm3);
		Radlen = Kapton->GetRadlen();
		std::cout<<"Gem KaptonFoil, Kapton: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitKaptonMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitKaptonMaterial);  //1

		//Gem Work Gas
		G4Material* ArGas = G4Material::GetMaterial("G4_Ar");
		G4Material* CO2Gas = G4Material::GetMaterial("G4_CARBON_DIOXIDE");
		double density = 0.001716043 *g/cm3;
		double fractionmass;
		int eln;
		G4Material* GemGas = new G4Material("GemGas",density,eln=2);
		GemGas->AddMaterial(ArGas,fractionmass=0.677959119);
		GemGas->AddMaterial(CO2Gas,fractionmass=0.322040881);
		Z = 0.;
		A = 0.;
		for(i=0; i<GemGas->GetElementVector()->size(); i++){    
			Z += (GemGas->GetElement(i)->GetZ())*
				(GemGas->GetFractionVector()[i]); 
			A += (GemGas->GetElement(i)->GetA())*	
				(GemGas->GetFractionVector()[i]);
		}

		Ionization = GemGas->GetIonisation()->GetMeanExcitationEnergy();
		Density = GemGas->GetDensity()/(g/cm3);
		Radlen = GemGas->GetRadlen();
		std::cout<<"Gem Work Gas, GemGas: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitGemGasMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitGemGasMaterial);  //2

		//Honeycomb
		density = 3.2e-2*g/cm3;
		int natoms;
		G4Material* Honeycomb = new G4Material("Nomex",density,eln=4);
		Honeycomb->AddElement(C,natoms=14);
		Honeycomb->AddElement(H,natoms=22);
		Honeycomb->AddElement(O,natoms=2);
		Honeycomb->AddElement(N,natoms=2);
		Z = 0.;
		A = 0.;
		for(i=0; i<Honeycomb->GetElementVector()->size(); i++){    
			Z += (Honeycomb->GetElement(i)->GetZ())*
				(Honeycomb->GetFractionVector()[i]); 
			A += (Honeycomb->GetElement(i)->GetA())*	
				(Honeycomb->GetFractionVector()[i]);
		}

		Ionization = Honeycomb->GetIonisation()->GetMeanExcitationEnergy();
		Density = Honeycomb->GetDensity()/(g/cm3);
		Radlen = Honeycomb->GetRadlen();
		std::cout<<"Gem Honeycomb, Honeycomb: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitHoneycombMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitHoneycombMaterial);  //3

		//Carbon Fiber
		density=1.57*g/cm3;
		G4Material* CarbonFiber=new G4Material("CarbonFiber",density,eln=3);
		CarbonFiber->AddElement(C,0.697);
		CarbonFiber->AddElement(H,0.0061);
		CarbonFiber->AddElement(O,0.2969);
		Z = 0.;
		A = 0.;
		for(i=0; i<CarbonFiber->GetElementVector()->size(); i++){    
			Z += (CarbonFiber->GetElement(i)->GetZ())*
				(CarbonFiber->GetFractionVector()[i]); 
			A += (CarbonFiber->GetElement(i)->GetA())*	
				(CarbonFiber->GetFractionVector()[i]);
		}

		Ionization = CarbonFiber->GetIonisation()->GetMeanExcitationEnergy();
		Density = CarbonFiber->GetDensity()/(g/cm3);
		Radlen = CarbonFiber->GetRadlen();
		std::cout<<"Gem CarbonFiber, CarbonFiber: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitCarbonFiberMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitCarbonFiberMaterial);  //4


		//Gem Foil Cu
		density = 7.2086555 *g/cm3;
		G4Material* GemCu = new G4Material("GemCu",density,eln=2);
		GemCu->AddMaterial(GemGas,fractionmass=4.654e-5);
		GemCu->AddMaterial(Cu,fractionmass=0.99995346);
		Z = 0.;
		A = 0.;
		for(i=0; i<GemCu->GetElementVector()->size(); i++){    
			Z += (GemCu->GetElement(i)->GetZ())*
				(GemCu->GetFractionVector()[i]); 
			A += (GemCu->GetElement(i)->GetA())*	
				(GemCu->GetFractionVector()[i]);
		}

		Ionization = GemCu->GetIonisation()->GetMeanExcitationEnergy();
		Density = GemCu->GetDensity()/(g/cm3);
		Radlen = GemCu->GetRadlen();
		std::cout<<"Gem Foil Cu, GemCu: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitGemCuMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitGemCuMaterial);  //5

		//Gem Foil Kapton
		density = 1.1427255 *g/cm3;
		G4Material* GemKapton = new G4Material("GemKapton",density,eln=2);
		GemKapton->AddMaterial(GemGas,fractionmass=0.00029358);
		GemKapton->AddMaterial(Kapton,fractionmass=0.99970642);
		Z = 0.;
		A = 0.;
		for(i=0; i<GemKapton->GetElementVector()->size(); i++){    
			Z += (GemKapton->GetElement(i)->GetZ())*
				(GemKapton->GetFractionVector()[i]); 
			A += (GemKapton->GetElement(i)->GetA())*	
				(GemKapton->GetFractionVector()[i]);
		}

		Ionization = GemKapton->GetIonisation()->GetMeanExcitationEnergy();
		Density = GemKapton->GetDensity()/(g/cm3);
		Radlen = GemKapton->GetRadlen();
		std::cout<<"Gem Foil Kapton, GemKapton: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitGemKaptonMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitGemKaptonMaterial);  //6

		//Al Foil
		G4Material* Al = G4Material::GetMaterial("G4_Al");
		Z = 0.;
		A = 0.;
		for(i=0; i<Al->GetElementVector()->size(); i++){    
			Z += (Al->GetElement(i)->GetZ())*
				(Al->GetFractionVector()[i]); 
			A += (Al->GetElement(i)->GetA())*	
				(Al->GetFractionVector()[i]);
		}

		Ionization = Al->GetIonisation()->GetMeanExcitationEnergy();
		Density = Al->GetDensity()/(g/cm3);
		Radlen = Al->GetRadlen();
		std::cout<<"Gem AlFoil, Al: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitAlMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitAlMaterial);  //7


		//Anode Cu  
		density = 8.270212135 *g/cm3;
		G4Material* AnodeCu = new G4Material("AnodeCu",density,eln=2);
		AnodeCu->AddMaterial(GemGas,fractionmass=1.59772e-5);
		AnodeCu->AddMaterial(Cu,fractionmass=0.9999840228);
		Z = 0.;
		A = 0.;
		for(i=0; i<AnodeCu->GetElementVector()->size(); i++){    
			Z += (AnodeCu->GetElement(i)->GetZ())*
				(AnodeCu->GetFractionVector()[i]); 
			A += (AnodeCu->GetElement(i)->GetA())*	
				(AnodeCu->GetFractionVector()[i]);
		}

		Ionization = AnodeCu->GetIonisation()->GetMeanExcitationEnergy();
		Density = AnodeCu->GetDensity()/(g/cm3);
		Radlen = AnodeCu->GetRadlen();
		std::cout<<"Gem Foil Cu, AnodeCu: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitAnodeCuMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitAnodeCuMaterial);  //8

		//Au Foil
		G4Material* Au = G4Material::GetMaterial("G4_Au");
		Z = 0.;
		A = 0.;
		for(i=0; i<Au->GetElementVector()->size(); i++){    
			Z += (Au->GetElement(i)->GetZ())*
				(Au->GetFractionVector()[i]); 
			A += (Au->GetElement(i)->GetA())*	
				(Au->GetFractionVector()[i]);
		}

		Ionization = Au->GetIonisation()->GetMeanExcitationEnergy();
		Density = Au->GetDensity()/(g/cm3);
		Radlen = Au->GetRadlen();
		std::cout<<"Anode AuFoil, Au: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitAuMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitAuMaterial);  //9

		//Epoxy  
		density = 1.25*g/cm3;
		//int natoms;
		G4Material* Epoxy = new G4Material("Epoxy",density,eln=3);
		Epoxy->AddElement(C,natoms=18);
		Epoxy->AddElement(H,natoms=31);
		Epoxy->AddElement(O,natoms=3);
		Z = 0.;
		A = 0.;
		for(i=0; i<Epoxy->GetElementVector()->size(); i++){    
			Z += (Epoxy->GetElement(i)->GetZ())*
				(Epoxy->GetFractionVector()[i]); 
			A += (Epoxy->GetElement(i)->GetA())*	
				(Epoxy->GetFractionVector()[i]);
		}

		Ionization = Epoxy->GetIonisation()->GetMeanExcitationEnergy();
		Density = Epoxy->GetDensity()/(g/cm3);
		Radlen = Epoxy->GetRadlen();
		std::cout<<"Anode Epoxy, Epoxy: Z: "<<Z<<" A: "<<(A/(g/mole))<<" Ionization: "<<(Ionization/eV)<<" Density: "<<Density<<" Radlen: "<<Radlen<<std::endl;
		KalFitMaterial FitEpoxyMaterial(Z,A/g/mole,Ionization/eV,Density,Radlen/10.);
		m_CGEMMaterials.push_back(FitEpoxyMaterial);  //10


		// Begin to construct Gem
		int nofLayer=gem->GemLayerNo();

		int nofGemFoil=gem->GemLayer(0).GemFoilNo();

		double anodeIR,anodeOR,gemSizeZ;
		double catKptIR,catKptOR,cathodeIR,cathodeOR,catHcIR,catHcOR;
		double cfIR,cfOR,cfHcIR,cfHcOR,layerIR,layerOR;
		double absorberIR,absorberOR,chamberIR,chamberOR;
		double sAuIR,sAuOR,anodKptIR,anodKptOR,anodEpoxyIR,anodEpoxyOR,shAuOR,shAuIR;
		double induceGasIR;


		for(int i=0;i<nofLayer;++i){

			std::cout<<"MyMessage::Construct Layer "<<i<<std::endl;

			if(imdc==11&&i==1) {

				anodeIR=gem->GemLayer(i).AnodeBR() *mm/cm;  //inner R of readout layer
				anodeOR=gem->GemLayer(i).R() *mm/cm;  //inner R of readout layer
				gemSizeZ=gem->GemLayer(i).GemSizeZ() *mm/cm;

				catKptIR=gem->GemLayer(i).CatInnKptIR() *mm/cm;
				catKptOR=gem->GemLayer(i).CatOutKptOR() *mm/cm;
				cathodeIR=gem->GemLayer(i).CathodeIR() *mm/cm;
				cathodeOR=gem->GemLayer(i).CathodeOR() *mm/cm;
				catHcIR=gem->GemLayer(i).CatInnKptOR() *mm/cm;
				catHcOR=gem->GemLayer(i).CatOutKptIR() *mm/cm;
				cfIR=gem->GemLayer(i).CFIR() *mm/cm;
				cfOR=gem->GemLayer(i).CFOR() *mm/cm;
				cfHcIR=gem->GemLayer(i).CfHcIR() *mm/cm;
				cfHcOR=gem->GemLayer(i).CfHcOR() *mm/cm;
				layerIR=cfIR;
				layerOR=gem->GemLayer(i).LayerOR() *mm/cm;

				absorberIR=gem->GemLayer(i).GemFoil(nofGemFoil-1).GemOR() *mm/cm;
				absorberOR=cathodeIR;

				sAuIR=gem->GemLayer(i).AnodSAuIR() *mm/cm;
				sAuOR=gem->GemLayer(i).AnodSAuOR() *mm/cm;
				anodKptIR=gem->GemLayer(i).AnodKptIR() *mm/cm;
				anodKptOR=gem->GemLayer(i).AnodKptOR() *mm/cm;
				anodEpoxyIR=gem->GemLayer(i).AnodCuOR() *mm/cm;
				anodEpoxyOR=anodKptIR;
				shAuOR=gem->GemLayer(i).AnodCuIR() *mm/cm;
				shAuIR=anodeIR;

				chamberIR=cfIR;
				chamberOR=cathodeOR;

				// add one layer of MDC air
				double ORLastLayer = m_CGEM.back().radius();
				if(cfIR>ORLastLayer)
				{
					radius=ORLastLayer;
					thick=cfIR-ORLastLayer;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"Mdc air: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					KalFitCylinder mdcAir(&_BesKalmanFitMaterials[0], radius, thick, length , z0);
					m_CGEM.push_back(mdcAir);
				}

				//inner Carbon Fiber
				radius=cfIR;
				thick=cfHcIR-cfIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"inner Carbon Fiber: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder innerCFCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(innerCFCylinder);
				KalFitCylinder innerCFCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(innerCFCylinder);

				//CF Honeycomb
				radius=cfHcIR;
				thick=cfHcOR-cfHcIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"out Honeycomb: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder outHoneycombCylinder(&_BesKalmanFitMaterials[10], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(outHoneycombCylinder);
				KalFitCylinder outHoneycombCylinder(&m_CGEMMaterials[3], radius, thick, length , z0);
				m_CGEM.push_back(outHoneycombCylinder);


				//out Carbon Fiber
				radius=cfHcOR;
				thick=cfOR-cfHcOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"out Carbon Fiber: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder outCFCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(outCFCylinder);
				KalFitCylinder outCFCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(outCFCylinder);


				//Anode Outer Au
				radius=shAuIR;
				thick=shAuOR-shAuIR;
				if(thick<0) { radius=radius+thick; thick*=-1.0;}
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Outer Au: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeOutAuCylinder(&_BesKalmanFitMaterials[16], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeOutAuCylinder);
				KalFitCylinder anodeOutAuCylinder(&m_CGEMMaterials[9], radius, thick, length , z0);
				m_CGEM.push_back(anodeOutAuCylinder);

				//Anode Outer Cu
				radius=shAuOR;
				thick=anodEpoxyIR-shAuOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Outer Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeOutCuCylinder(&_BesKalmanFitMaterials[15], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeOutCuCylinder);
				KalFitCylinder anodeOutCuCylinder(&m_CGEMMaterials[8], radius, thick, length , z0);
				m_CGEM.push_back(anodeOutCuCylinder);

				//Anode Epoxy
				radius=anodEpoxyIR;
				thick=anodEpoxyOR-anodEpoxyIR;
				if(thick<0) { radius=radius+thick; thick*=-1.0;}
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Epoxy: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeEpoxyCylinder(&_BesKalmanFitMaterials[17], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeEpoxyCylinder);
				KalFitCylinder anodeEpoxyCylinder(&m_CGEMMaterials[10], radius, thick, length , z0);
				m_CGEM.push_back(anodeEpoxyCylinder);

				//Anode Kapton
				radius=anodKptIR;
				thick=anodKptOR-anodKptIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeKaptonCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeKaptonCylinder);
				KalFitCylinder anodeKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(anodeKaptonCylinder);

				//Anode Inner Cu
				radius=anodKptOR;
				thick=sAuIR-anodKptOR;
				if(thick<0) { radius=radius+thick; thick*=-1.0;}
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeInnCuCylinder(&_BesKalmanFitMaterials[15], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeInnCuCylinder);
				KalFitCylinder anodeInnCuCylinder(&m_CGEMMaterials[8], radius, thick, length , z0);
				m_CGEM.push_back(anodeInnCuCylinder);

				//Anode Inner Au
				radius=sAuIR;
				thick=sAuOR-sAuIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Inner Au: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeInnAuCylinder(&_BesKalmanFitMaterials[16], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeInnAuCylinder);
				KalFitCylinder anodeInnAuCylinder(&m_CGEMMaterials[9], radius, thick, length , z0);
				m_CGEM.push_back(anodeInnAuCylinder);

				//Induce Gas
				double gemFirstR = gem->GemLayer(i).GemFoil(0).InnCuR() *mm/cm;
				radius=sAuOR;
				thick=gemFirstR-sAuOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"induce GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder induceGasCylinder(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(induceGasCylinder);
				KalFitCylinder induceGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
				m_CGEM.push_back(induceGasCylinder);

				for(int j=0;j<nofGemFoil;j++){

					std::cout<<"MyMessage::Construct Gem Foil "<<j<<std::endl;

					int nofGemLine=gem->GemLayer(i).GemFoil(j).GemLineNo();
					double gemR = gem->GemLayer(i).GemFoil(j).InnCuR() *mm/cm;
					double kaptonR=gem->GemLayer(i).GemFoil(j).KaptonR() *mm/cm;
					double outCuR = gem->GemLayer(i).GemFoil(j).OutCuR() *mm/cm;
					double gemOR = gem->GemLayer(i).GemFoil(j).GemOR() *mm/cm;

					//GemFoil Inner Cu
					radius=gemR;
					thick=kaptonR-gemR;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"gemFoil inner Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					KalFitCylinder gemFoilInnCuCylinder(&m_CGEMMaterials[5], radius, thick, length , z0);
					m_CGEM.push_back(gemFoilInnCuCylinder);

					//GemFoil Kapton
					radius=kaptonR;
					thick=outCuR-kaptonR;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"gemFoil Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					//KalFitCylinder gemFoilKaptonCylinder(&_BesKalmanFitMaterials[13], radius, thick, length , z0);
					//_BesKalmanFitWalls.push_back(gemFoilKaptonCylinder);
					KalFitCylinder gemFoilKaptonCylinder(&m_CGEMMaterials[6], radius, thick, length , z0);
					m_CGEM.push_back(gemFoilKaptonCylinder);

					//GemFoil Out Cu
					radius=outCuR;
					thick=gemOR-outCuR;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"gemFoil out Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					//KalFitCylinder gemFoilOutCuCylinder(&_BesKalmanFitMaterials[12], radius, thick, length , z0);
					//_BesKalmanFitWalls.push_back(gemFoilOutCuCylinder);
					KalFitCylinder gemFoilOutCuCylinder(&m_CGEMMaterials[5], radius, thick, length , z0);
					m_CGEM.push_back(gemFoilOutCuCylinder);

					if(j < nofGemFoil-1){
						double gemNextR = gem->GemLayer(i).GemFoil(j+1).InnCuR() *mm/cm;
						//Transfer Gas
						radius=gemOR;
						thick=gemNextR-gemOR;
						length=gemSizeZ;
						z0     = 0.0;
						std::cout<<"transfer GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
						//KalFitCylinder transferGasCylinder(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
						//_BesKalmanFitWalls.push_back(transferGasCylinder);
						KalFitCylinder transferGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
						m_CGEM.push_back(transferGasCylinder);
					}

				}

				//Drift Gas
				radius=absorberIR;
				thick=absorberOR-absorberIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"drift GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder driftGasCylinder(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(driftGasCylinder);
				KalFitCylinder driftGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
				m_CGEM.push_back(driftGasCylinder);

				//Cathode Inner Cu
				radius=cathodeIR;
				thick=catKptIR-cathodeIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeInnCuCylinder(&_BesKalmanFitMaterials[7], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeInnCuCylinder);
				KalFitCylinder cathodeInnCuCylinder(&m_CGEMMaterials[0], radius, thick, length , z0);
				m_CGEM.push_back(cathodeInnCuCylinder);

				//Cathode Inner Kapton
				radius=catKptIR;
				thick=catHcIR-catKptIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeInnKaptonCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeInnKaptonCylinder);
				KalFitCylinder cathodeInnKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(cathodeInnKaptonCylinder);

				//Inner Honeycomb
				radius=catHcIR;
				thick=catHcOR-catHcIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"inner Honeycomb: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder innerHoneycombCylinder(&_BesKalmanFitMaterials[10], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(innerHoneycombCylinder);
				KalFitCylinder innerHoneycombCylinder(&m_CGEMMaterials[3], radius, thick, length , z0);
				m_CGEM.push_back(innerHoneycombCylinder);

				//Cathode Outer Kapton
				radius=catHcOR;
				thick=catKptOR-catHcOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeOutKaptonCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeOutKaptonCylinder);
				KalFitCylinder cathodeOutKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(cathodeOutKaptonCylinder);

				//Cathode Outer Cu
				radius=catKptOR;
				thick=cathodeOR-catKptOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeOutCuCylinder(&_BesKalmanFitMaterials[7], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeOutCuCylinder);
				KalFitCylinder cathodeOutCuCylinder(&m_CGEMMaterials[0], radius, thick, length , z0);
				m_CGEM.push_back(cathodeOutCuCylinder);

				//air between layer
				radius=chamberOR;
				thick=layerOR-chamberOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"layer gap Air: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder layerAirCylinder(&_BesKalmanFitMaterials[2], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(layerAirCylinder);
				KalFitCylinder layerAirCylinder(&_BesKalmanFitMaterials[4], radius, thick, length , z0);
				m_CGEM.push_back(layerAirCylinder);


			}// layer 2
			else {
				anodeIR=gem->GemLayer(i).R() *mm/cm;  //inner R of readout layer
				anodeOR=gem->GemLayer(i).AnodeBR() *mm/cm;  //inner R of readout layer
				gemSizeZ=gem->GemLayer(i).GemSizeZ() *mm/cm;

				catKptIR=gem->GemLayer(i).CatInnKptIR() *mm/cm;
				catKptOR=gem->GemLayer(i).CatOutKptOR() *mm/cm;
				cathodeIR=gem->GemLayer(i).CathodeIR() *mm/cm;
				cathodeOR=gem->GemLayer(i).CathodeOR() *mm/cm;
				catHcIR=gem->GemLayer(i).CatInnKptOR() *mm/cm;
				catHcOR=gem->GemLayer(i).CatOutKptIR() *mm/cm;
				cfIR=gem->GemLayer(i).CFIR() *mm/cm;
				cfOR=gem->GemLayer(i).CFOR() *mm/cm;
				cfHcIR=gem->GemLayer(i).CfHcIR() *mm/cm;
				cfHcOR=gem->GemLayer(i).CfHcOR() *mm/cm;
				layerIR=cathodeIR;
				layerOR=gem->GemLayer(i).LayerOR() *mm/cm;

				absorberIR=cathodeOR;
				absorberOR=gem->GemLayer(i).GemFoil(0).InnCuR() *mm/cm;

				sAuIR=gem->GemLayer(i).AnodSAuIR() *mm/cm;
				sAuOR=gem->GemLayer(i).AnodSAuOR() *mm/cm;
				anodKptIR=gem->GemLayer(i).AnodKptIR() *mm/cm;
				anodKptOR=gem->GemLayer(i).AnodKptOR() *mm/cm;
				anodEpoxyIR=anodKptOR;
				anodEpoxyOR=gem->GemLayer(i).AnodCuIR() *mm/cm;
				shAuIR=gem->GemLayer(i).AnodCuOR() *mm/cm;
				shAuOR=anodeOR;

				chamberIR=cathodeIR;
				chamberOR=cfOR;

				induceGasIR = gem->GemLayer(i).GemFoil(nofGemFoil-1).GemOR() *mm/cm;
				
				// add one layer of MDC air
				if(m_CGEM.size()>0)
				{
					double ORLastLayer = m_CGEM.back().radius();
					if(cathodeIR>ORLastLayer)
					{
						radius=ORLastLayer;
						thick=cathodeIR-ORLastLayer;
						length=gemSizeZ;
						z0     = 0.0;
						std::cout<<"Mdc air: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
						KalFitCylinder mdcAir(&_BesKalmanFitMaterials[0], radius, thick, length , z0);
						m_CGEM.push_back(mdcAir);
					}
				}


				//Cathode Inner Cu
				radius=cathodeIR;
				thick=catKptIR-cathodeIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeInnCuCylinder(&_BesKalmanFitMaterials[7], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeInnCuCylinder);
				KalFitCylinder cathodeInnCuCylinder(&m_CGEMMaterials[0], radius, thick, length , z0);
				m_CGEM.push_back(cathodeInnCuCylinder);

				//Cathode Inner Kapton
				radius=catKptIR;
				thick=catHcIR-catKptIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeInnKaptonCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeInnKaptonCylinder);
				KalFitCylinder cathodeInnKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(cathodeInnKaptonCylinder);

				//Inner Honeycomb
				radius=catHcIR;
				thick=catHcOR-catHcIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"inner Honeycomb: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder innerHoneycombCylinder(&_BesKalmanFitMaterials[10], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(innerHoneycombCylinder);
				KalFitCylinder innerHoneycombCylinder(&m_CGEMMaterials[3], radius, thick, length , z0);
				m_CGEM.push_back(innerHoneycombCylinder);

				//Cathode Outer Kapton
				radius=catHcOR;
				thick=catKptOR-catHcOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeOutKaptonCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeOutKaptonCylinder);
				KalFitCylinder cathodeOutKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(cathodeOutKaptonCylinder);

				//Cathode Outer Cu
				radius=catKptOR;
				thick=cathodeOR-catKptOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"cathode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder cathodeOutCuCylinder(&_BesKalmanFitMaterials[7], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(cathodeOutCuCylinder);
				KalFitCylinder cathodeOutCuCylinder(&m_CGEMMaterials[0], radius, thick, length , z0);
				m_CGEM.push_back(cathodeOutCuCylinder);

				//Drift Gas
				radius=absorberIR;
				thick=absorberOR-absorberIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"drift GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder driftGasCylinder(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(driftGasCylinder);
				KalFitCylinder driftGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
				m_CGEM.push_back(driftGasCylinder);

				for(int j=0;j<nofGemFoil;j++){

					std::cout<<"MyMessage::Construct Gem Foil "<<j<<std::endl;

					int nofGemLine=gem->GemLayer(i).GemFoil(j).GemLineNo();
					double gemR = gem->GemLayer(i).GemFoil(j).InnCuR()/cm;
					double kaptonR=gem->GemLayer(i).GemFoil(j).KaptonR()/cm;
					double outCuR = gem->GemLayer(i).GemFoil(j).OutCuR()/cm;
					double gemOR = gem->GemLayer(i).GemFoil(j).GemOR()/cm;

					//GemFoil Inner Cu
					radius=gemR;
					thick=kaptonR-gemR;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"gemFoil inner Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					//KalFitCylinder gemFoilInnCuCylinder(&_BesKalmanFitMaterials[12], radius, thick, length , z0);
					//_BesKalmanFitWalls.push_back(gemFoilInnCuCylinder);
					KalFitCylinder gemFoilInnCuCylinder(&m_CGEMMaterials[5], radius, thick, length , z0);
					m_CGEM.push_back(gemFoilInnCuCylinder);

					//GemFoil Kapton
					radius=kaptonR;
					thick=outCuR-kaptonR;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"gemFoil Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					//KalFitCylinder gemFoilKaptonCylinder(&_BesKalmanFitMaterials[13], radius, thick, length , z0);
					//_BesKalmanFitWalls.push_back(gemFoilKaptonCylinder);
					KalFitCylinder gemFoilKaptonCylinder(&m_CGEMMaterials[6], radius, thick, length , z0);
					m_CGEM.push_back(gemFoilKaptonCylinder);

					//GemFoil Out Cu
					radius=outCuR;
					thick=gemOR-outCuR;
					length=gemSizeZ;
					z0     = 0.0;
					std::cout<<"gemFoil out Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
					//KalFitCylinder gemFoilOutCuCylinder(&_BesKalmanFitMaterials[12], radius, thick, length , z0);
					//_BesKalmanFitWalls.push_back(gemFoilOutCuCylinder);
					KalFitCylinder gemFoilOutCuCylinder(&m_CGEMMaterials[5], radius, thick, length , z0);
					m_CGEM.push_back(gemFoilOutCuCylinder);

					if(j < nofGemFoil-1){
						//Transfer Gas
						double gemNextR = gem->GemLayer(i).GemFoil(j+1).InnCuR() *mm/cm;
						radius=gemOR;
						thick=gemNextR-gemOR;
						length=gemSizeZ;
						z0     = 0.0;
						std::cout<<"transfer GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
						//KalFitCylinder transferGasCylinder(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
						//_BesKalmanFitWalls.push_back(transferGasCylinder);
						KalFitCylinder transferGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
						m_CGEM.push_back(transferGasCylinder);
					}

				}


				//Induce Gas
				radius=induceGasIR;
				thick=sAuIR-induceGasIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"induce GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder induceGasCylinder(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(induceGasCylinder);
				KalFitCylinder induceGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
				m_CGEM.push_back(induceGasCylinder);

				//Anode Inner Au
				radius=sAuIR;
				thick=sAuOR-sAuIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Inner Au: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeInnAuCylinder(&_BesKalmanFitMaterials[16], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeInnAuCylinder);
				KalFitCylinder anodeInnAuCylinder(&m_CGEMMaterials[9], radius, thick, length , z0);
				m_CGEM.push_back(anodeInnAuCylinder);

				//Anode Inner Cu
				radius=sAuOR;
				thick=anodKptIR-sAuOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeInnCuCylinder(&_BesKalmanFitMaterials[15], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeInnCuCylinder);
				KalFitCylinder anodeInnCuCylinder(&m_CGEMMaterials[8], radius, thick, length , z0);
				m_CGEM.push_back(anodeInnCuCylinder);

				//Anode Kapton
				radius=anodKptIR;
				thick=anodKptOR-anodKptIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeKaptonCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeKaptonCylinder);
				KalFitCylinder anodeKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(anodeKaptonCylinder);

				//Anode Epoxy
				radius=anodEpoxyIR;
				thick=anodEpoxyOR-anodEpoxyIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Epoxy: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeEpoxyCylinder(&_BesKalmanFitMaterials[17], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeEpoxyCylinder);
				KalFitCylinder anodeEpoxyCylinder(&m_CGEMMaterials[10], radius, thick, length , z0);
				m_CGEM.push_back(anodeEpoxyCylinder);

				//Anode Outer Cu
				radius=anodEpoxyOR;
				thick=shAuIR-anodEpoxyOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Outer Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeOutCuCylinder(&_BesKalmanFitMaterials[15], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeOutCuCylinder);
				KalFitCylinder anodeOutCuCylinder(&m_CGEMMaterials[8], radius, thick, length , z0);
				m_CGEM.push_back(anodeOutCuCylinder);

				//Anode Outer Au
				radius=shAuIR;
				thick=shAuOR-shAuIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"anode Outer Au: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder anodeOutAuCylinder(&_BesKalmanFitMaterials[16], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(anodeOutAuCylinder);
				KalFitCylinder anodeOutAuCylinder(&m_CGEMMaterials[9], radius, thick, length , z0);
				m_CGEM.push_back(anodeOutAuCylinder);

				//inner Carbon Fiber
				radius=cfIR;
				thick=cfHcIR-cfIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"inner Carbon Fiber: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder innerCFCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(innerCFCylinder);
				KalFitCylinder innerCFCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(innerCFCylinder);

				//CF Honeycomb
				radius=cfHcIR;
				thick=cfHcOR-cfHcIR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"out Honeycomb: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder outHoneycombCylinder(&_BesKalmanFitMaterials[10], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(outHoneycombCylinder);
				KalFitCylinder outHoneycombCylinder(&m_CGEMMaterials[3], radius, thick, length , z0);
				m_CGEM.push_back(outHoneycombCylinder);


				//out Carbon Fiber
				radius=cfHcOR;
				thick=cfOR-cfHcOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"out Carbon Fiber: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder outCFCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(outCFCylinder);
				KalFitCylinder outCFCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
				m_CGEM.push_back(outCFCylinder);

				//air between layer
				radius=chamberOR;
				thick=layerOR-chamberOR;
				length=gemSizeZ;
				z0     = 0.0;
				std::cout<<"layer gap Air: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
				//KalFitCylinder layerAirCylinder(&_BesKalmanFitMaterials[2], radius, thick, length , z0);
				//_BesKalmanFitWalls.push_back(layerAirCylinder);
				KalFitCylinder layerAirCylinder(&_BesKalmanFitMaterials[4], radius, thick, length , z0);
				m_CGEM.push_back(layerAirCylinder);

			}

		}// loop layers


	}// end of CGEM
	else // MDC
	{
		/// film1 of the innerwall of inner drift chamber
		radius = innerwallFilm1Tub->GetInnerRadius()/(cm);
		thick  = innerwallFilm1Tub->GetOuterRadius()/(cm) - innerwallFilm1Tub->GetInnerRadius()/(cm);
		length = 2.0*innerwallFilm1Tub->GetZHalfLength()/(cm);
		z0     = 0.0;
		std::cout<<"innerwallFilm1: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
		KalFitCylinder innerwallFilm1Cylinder(&_BesKalmanFitMaterials[1], radius, thick, length , z0);
		_BesKalmanFitWalls.push_back(innerwallFilm1Cylinder);


		/// innerwall of inner drift chamber
		radius = innerwallTub->GetInnerRadius()/(cm);
		thick  = innerwallTub->GetOuterRadius()/(cm) - innerwallTub->GetInnerRadius()/(cm);
		length = 2.0*innerwallTub->GetZHalfLength()/(cm);
		z0     = 0.0;
		std::cout<<"innerwall: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
		KalFitCylinder innerwallCylinder(&_BesKalmanFitMaterials[2], radius, thick, length , z0);
		_BesKalmanFitWalls.push_back(innerwallCylinder);

		/// film0 of the innerwall of inner drift chamber
		radius = innerwallFilm0Tub->GetInnerRadius()/(cm);
		thick  = innerwallFilm0Tub->GetOuterRadius()/(cm) - innerwallFilm0Tub->GetInnerRadius()/(cm);
		length = 2.0*innerwallFilm0Tub->GetZHalfLength()/(cm);
		z0     = 0.0;
		std::cout<<"innerwallFilm0: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
		KalFitCylinder innerwallFilm0Cylinder(&_BesKalmanFitMaterials[3], radius, thick, length , z0);
		_BesKalmanFitWalls.push_back(innerwallFilm0Cylinder);
	}

	/// outer air, be attention the calculation of the radius and thick of the air cylinder is special 
	radius = outerBeTub->GetOuterRadius()/(cm);
	thick  = innerwallTub->GetInnerRadius()/(cm) - outerBeTub->GetOuterRadius()/(cm);
	length = 2.0*innerwallTub->GetZHalfLength()/(cm);
	z0     = 0.0;
	std::cout<<"outer air: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
	KalFitCylinder outerAirCylinder(&_BesKalmanFitMaterials[4], radius, thick, length , z0);
	_BesKalmanFitWalls.push_back(outerAirCylinder);

	/// outer Beryllium layer
	radius = outerBeTub->GetInnerRadius()/(cm);
	thick  = outerBeTub->GetOuterRadius()/(cm) - outerBeTub->GetInnerRadius()/(cm);
	length = 2.0*outerBeTub->GetZHalfLength()/(cm);
	z0     = 0.0;
	std::cout<<"outer Be: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl; 
	KalFitCylinder outerBeCylinder(&_BesKalmanFitMaterials[5], radius, thick, length , z0);
	_BesKalmanFitWalls.push_back(outerBeCylinder);

	/// oil layer
	radius = oilLayerTub->GetInnerRadius()/(cm);
	thick  = oilLayerTub->GetOuterRadius()/(cm) - oilLayerTub->GetInnerRadius()/(cm);
	length = 2.0*oilLayerTub->GetZHalfLength()/(cm);
	z0     = 0.0;
	std::cout<<"oil layer: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl; 
	KalFitCylinder oilLayerCylinder(&_BesKalmanFitMaterials[6], radius, thick, length , z0);
	_BesKalmanFitWalls.push_back(oilLayerCylinder);

	/// inner Beryllium layer
	radius = innerBeTub->GetInnerRadius()/(cm);
	thick  = innerBeTub->GetOuterRadius()/(cm) - innerBeTub->GetInnerRadius()/(cm);
	length = 2.0*innerBeTub->GetZHalfLength()/(cm);
	z0     = 0.0;
	std::cout<<"inner Be: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl; 
	KalFitCylinder innerBeCylinder(&_BesKalmanFitMaterials[7], radius, thick, length , z0);
	_BesKalmanFitWalls.push_back(innerBeCylinder);

	/// gold layer
	radius = goldLayerTub->GetInnerRadius()/(cm);
	thick  = goldLayerTub->GetOuterRadius()/(cm) - goldLayerTub->GetInnerRadius()/(cm);
	length = 2.0*goldLayerTub->GetZHalfLength()/(cm);
	z0     = 0.0;
	std::cout<<"gold layer: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl; 
	KalFitCylinder goldLayerCylinder(&_BesKalmanFitMaterials[8], radius, thick, length , z0);
	_BesKalmanFitWalls.push_back(goldLayerCylinder);

// >>>>>>>
	/// add one layer of CDC gas by wuchen 2013-06-23
	radius=50.2*cm;
	thick=30.4*cm;
	length=75*cm; // this is half length
	z0     = 0.0*cm;
	std::cout<<"Mdc air: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
	KalFitCylinder mdcAir(&_BesKalmanFitMaterials[9], radius, thick, length , z0);
	m_CGEM.push_back(mdcAir);
	/// CDC inner wall by wuchen 2013-06-23
	radius = 50.16; // cm
	thick  = 0.04; // cm
	length = 150; // cm
	z0     = 0.0; // cm
	std::cout<<"CDC inner wall: "<<" radius: "<<radius<<" thick:"<<thick<<" outerRadius:"<<radius+thick<<" length: "<<length<<std::endl;
	KalFitCylinder outerAirCylinder(&_BesKalmanFitMaterials[10], radius, thick, length , z0);
	_BesKalmanFitWalls.push_back(outerAirCylinder);
// <<<<<<<
}



/*
// Begin to construct Gem
BesMdcGemGeo* gem=BesMdcGemGeo::GetGeo();
int nofLayer=gem->GemLayerNo();

int nofGemFoil=gem->GemLayer(0).GemFoilNo();

double gapTransfer=gem->GemLayer(0).GapT() *mm/cm;
double gapDrift=gem->GemLayer(0).GapD() *mm/cm;
double gapInduce=gem->GemLayer(0).GapI() *mm/cm;
double gapCF=gem->GemLayer(0).GapCF() *mm/cm;
double gapLayer=gem->GemLayer(0).GapLayer() *mm/cm;
double gemSpanAngle=360 *deg;

double holePitchZ=gem->GemLayer(0).HolePitch() *um/cm;
double holePitchPhi=sqrt(3)*holePitchZ;
double copperSizeR=gem->GemLayer(0).CuTh() *um/cm;
double alShieldingSizeR=gem->GemLayer(0).AlTh() *um/cm;
double kaptonSizeR=gem->GemLayer(0).KaptonTh() *um/cm;
double cfSizeR=gem->GemLayer(0).CarbonfiberTh() *um/cm;
double honeycombSizeR=gem->GemLayer(0).HoneycombTh() *mm/cm;

double gemFoilSizeR=kaptonSizeR+2*copperSizeR;
double cathodSizeR=kaptonSizeR+copperSizeR;
double anodeSizeR=2*kaptonSizeR+copperSizeR+alShieldingSizeR;
double cfSsSizeR=2*cfSizeR+honeycombSizeR;

for(int i=0;i<nofLayer;++i){

std::cout<<"MyMessage::Construct Layer "<<i<<std::endl;

double anodeR=gem->GemLayer(i).R() *mm/cm;  //inner R of readout layer
double gemSizeZ=gem->GemLayer(i).L() *mm/cm;

double cathodR=anodeR-gapInduce-2*gapTransfer-gapDrift-3*gemFoilSizeR-cathodSizeR;
double innHoneycombR=cathodR-honeycombSizeR;
double cfSsR=anodeR+anodeSizeR;
double honeycombR=anodeR+anodeSizeR+cfSizeR;

//Inner Honeycomb
radius=innHoneycombR;
thick=honeycombSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"inner Honeycomb: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder innerHoneycombCylinder(&m_CGEMMaterials[3], radius, thick, length , z0);
m_CGEM.push_back(innerHoneycombCylinder);

//Cathode Kapton
radius=cathodR;
thick=kaptonSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"cathode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder cathodeKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
m_CGEM.push_back(cathodeKaptonCylinder);

//Cathode Cu
radius=cathodR+kaptonSizeR;
thick=copperSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"cathode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder cathodeCuCylinder(&m_CGEMMaterials[0], radius, thick, length , z0);
m_CGEM.push_back(cathodeCuCylinder);

//Drift Gas
radius=cathodR+cathodSizeR;
thick=gapDrift;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"drift GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder driftGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
m_CGEM.push_back(driftGasCylinder);

double gemR=anodeR-gapInduce-2*gapTransfer-3*gemFoilSizeR;
for(int j=0;j<nofGemFoil;j++){

	std::cout<<"MyMessage::Construct Gem Foil "<<j<<std::endl;

	double kaptonR=gemR+copperSizeR;
	double holePosRho=gemR+gemFoilSizeR/2;

	//GemFoil Inner Cu
	radius=gemR;
	thick=copperSizeR;
	length=gemSizeZ;
	z0     = 0.0;
	std::cout<<"gemFoil inner Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
	KalFitCylinder gemFoilInnCuCylinder(&m_CGEMMaterials[5], radius, thick, length , z0);
	m_CGEM.push_back(gemFoilInnCuCylinder);

	//GemFoil Kapton
	radius=kaptonR;
	thick=kaptonSizeR;
	length=gemSizeZ;
	z0     = 0.0;
	std::cout<<"gemFoil Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
	//KalFitCylinder gemFoilKaptonCylinder(&m_CGEMMaterials[6], radius, thick, length , z0);
	KalFitCylinder gemFoilKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
	if(j==0) gemFoilKaptonCylinder.setSensitive();
	m_CGEM.push_back(gemFoilKaptonCylinder);

	//GemFoil Out Cu
	radius=kaptonR+kaptonSizeR;
	thick=copperSizeR;
	length=gemSizeZ;
	z0     = 0.0;
	std::cout<<"gemFoil out Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
	KalFitCylinder gemFoilOutCuCylinder(&m_CGEMMaterials[5], radius, thick, length , z0);
	m_CGEM.push_back(gemFoilOutCuCylinder);

	if(j < nofGemFoil-1){
		//Transfer Gas
		radius=gemR+gemFoilSizeR;
		thick=gapTransfer;
		length=gemSizeZ;
		z0     = 0.0;
		std::cout<<"transfer GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
		KalFitCylinder transferGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
		m_CGEM.push_back(transferGasCylinder);
	}

	gemR += gapTransfer+gemFoilSizeR;

}


//Induce Gas
radius=anodeR-gapInduce;
thick=gapInduce;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"induce GemGas: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder induceGasCylinder(&m_CGEMMaterials[2], radius, thick, length , z0);
m_CGEM.push_back(induceGasCylinder);

//Anode Cu
radius=anodeR;
thick=copperSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"anode Cu: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder anodeCuCylinder(&m_CGEMMaterials[0], radius, thick, length , z0);
m_CGEM.push_back(anodeCuCylinder);

//Anode Kapton
radius=anodeR+copperSizeR;
thick=2*kaptonSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"anode Kapton: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder anodeKaptonCylinder(&m_CGEMMaterials[1], radius, thick, length , z0);
m_CGEM.push_back(anodeKaptonCylinder);

//Anode Al Shielding
radius=anodeR+anodeSizeR-alShieldingSizeR;
thick=alShieldingSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"anode Al: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder anodeAlCylinder(&m_CGEMMaterials[7], radius, thick, length , z0);
m_CGEM.push_back(anodeAlCylinder);

//inner Carbon Fiber
radius=cfSsR;
thick=cfSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"inner Carbon Fiber: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder innerCFCylinder(&m_CGEMMaterials[4], radius, thick, length , z0);
m_CGEM.push_back(innerCFCylinder);

//Out Honeycomb
radius=honeycombR;
thick=honeycombSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"out Honeycomb: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder outHoneycombCylinder(&m_CGEMMaterials[3], radius, thick, length , z0);
m_CGEM.push_back(outHoneycombCylinder);


//out Carbon Fiber
radius=honeycombR+honeycombSizeR;
thick=cfSizeR;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"out Carbon Fiber: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder outCFCylinder(&m_CGEMMaterials[4], radius, thick, length , z0);
m_CGEM.push_back(outCFCylinder);

//air between layer
radius=cfSsR+cfSsSizeR;
thick=innHoneycombR+gapLayer-radius;
length=gemSizeZ;
z0     = 0.0;
std::cout<<"layer gap Air: "<<" radius: "<<radius<<" thick:"<<thick<<" length: "<<length<<std::endl;
KalFitCylinder layerAirCylinder(&_BesKalmanFitMaterials[4], radius, thick, length , z0);
m_CGEM.push_back(layerAirCylinder);
}*/


