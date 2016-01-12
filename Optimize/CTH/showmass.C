{
	// constant
	double pi = 3.14159265358;

	// in kg/cm3
	double d_sci = 1.032e-3;
	double d_che = 1.18e-3;
	double d_pmt = 0.95e-3;
	double d_shd = 11.35e-3; // Lead
//	double d_shd = 19.3e-3;  // Tungsten
	double d_sup = 8e-3;     // SUS
//	double d_sup = 2.699e-3; // Aluminium

	// in cm3
	double v_sci = 0;
	double v_che = 0;
	double v_shd = 0;
	double v_sup = 0;
	double v_pmt = 0;
	double v_lg = 0;

// upstream;
	v_sci = 9*.5*30; v_sci*=64;
	v_che = 9*1*30; v_che*=64;
	v_pmt = 8*pi*2.25*2.25; v_pmt*=128;
	v_lg = 9*(0.6+1)*12*64;
	v_shd = 30*pi*2*42.0714*1.6;
	v_shd += pi*(pow(49.94,2)-pow(40.4714,2))*0.8;
	v_sup = 59*pi*2*42.0714*0.5;
	v_sup += pi*(pow(90,2)-pow(42.0714,2))*0.5;
	v_sup += pi*(pow(72.2714,2)-pow(49.94,2))*0.5;
	v_sup += 9*pi*2*72.714*0.5;

	double m_sci = v_sci*d_sci;
	double m_che = v_che*d_che;
	double m_lg = v_lg*d_che;
	double m_pmt = v_pmt*d_pmt;
	double m_shd = v_shd*d_shd;
	double m_sup = v_sup*d_sup;
	std::cout<<"Upstream:"<<std::endl;
	std::cout<<"  CTH:          "<<m_sci+m_che+m_pmt+m_lg<<" kg"<<std::endl;
	std::cout<<"    Scintillator: "<<m_sci<<" kg"<<std::endl;
	std::cout<<"    Cherenkov:    "<<m_che<<" kg"<<std::endl;
	std::cout<<"    PMT:          "<<m_pmt<<" kg"<<std::endl;
	std::cout<<"    Light guide:  "<<m_lg<<" kg"<<std::endl;
	std::cout<<"  Shielding:    "<<m_shd<<" kg"<<std::endl;
	std::cout<<"  Supportor:    "<<m_sup<<" kg"<<std::endl;

// downstream;
	v_sci = 9*.5*30; v_sci*=64;
	v_che = 9*1*30; v_che*=64;
	v_pmt = 8*pi*2.25*2.25; v_pmt*=128;
	v_lg = 9*(0.6+1)*31.3*64;
	v_shd = 30*pi*2*42.0714*1.6;
	v_shd += pi*(pow(49.94,2)-pow(40.4714,2))*0.8;
	v_sup = 81.3*pi*2*42.0714*0.5;
	v_sup += pi*(pow(90,2)-pow(42.0714,2))*0.5;
	v_sup += pi*(pow(72.2714,2)-pow(49.94,2))*0.5;
	v_sup += 9*pi*2*72.714*0.5;

	double m_sci = v_sci*d_sci;
	double m_che = v_che*d_che;
	double m_lg = v_lg*d_che;
	double m_pmt = v_pmt*d_pmt;
	double m_shd = v_shd*d_shd;
	double m_sup = v_sup*d_sup;
	std::cout<<"Downstream:"<<std::endl;
	std::cout<<"  CTH:          "<<m_sci+m_che+m_pmt+m_lg<<" kg"<<std::endl;
	std::cout<<"    Scintillator: "<<m_sci<<" kg"<<std::endl;
	std::cout<<"    Cherenkov:    "<<m_che<<" kg"<<std::endl;
	std::cout<<"    PMT:          "<<m_pmt<<" kg"<<std::endl;
	std::cout<<"    Light guide:  "<<m_lg<<" kg"<<std::endl;
	std::cout<<"  Shielding:    "<<m_shd<<" kg"<<std::endl;
	std::cout<<"  Supportor:    "<<m_sup<<" kg"<<std::endl;
}
