#	kWhite =0,   kBlack =1,   kGray=920,
#	kRed   =632, kGreen =416, kBlue=600, kYellow=400, kMagenta=616, kCyan=432,
#	kOrange=800, kSpring=820, kTeal=840, kAzure =860, kViolet =880, kPink=900
###########################################################################################################################

#	TYPE     | Name
	TreeName | t

#	TYPE    | Path
#	oFILE   | /home/chen/MyWorkArea/MyStudies/Beam/result/g60cm6mm/Andy/ts2_0.em.root

#	TYPE    | Name                         | Type      | Vector?
#	                                0:double;1:int;string:2
	TBranch | x                            | 0         | 0
	TBranch | y                            | 0         | 0
	TBranch | z                            | 0         | 0
	TBranch | px                           | 0         | 0
	TBranch | py                           | 0         | 0
	TBranch | pz                           | 0         | 0
	TBranch | t                            | 0         | 0
	TBranch | ox                           | 0         | 0
	TBranch | oy                           | 0         | 0
	TBranch | oz                           | 0         | 0
	TBranch | pid                          | 1         | 0

#	TYPE    | NAME            | TITLE | xAxisName    | yAxisName     | BIN | LEFT  | RIGHT | minX | minY  |COL|COM|LOGX|LOGY|Marker|Norm |DrawOption
	TH1D    | pa              |       | p_{a}(MeV/c) | probability   | 100 | 0     | 2    | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | pz              |       | p_{z}(MeV/c) | probability   | 100 | 0     | 2    | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | pt              |       | p_{t}(MeV/c) | probability   | 100 | 0     | 2    | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | gTime           |       | time(ns)     | probability   | 100 | 0     | 20    | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | pa_tail         |       | p_{a}(MeV/c) | probability   | 100 | 0     | 300 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | pz_tail         |       | p_{z}(MeV/c) | probability   | 100 | 0     | 300 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | pt_tail         |       | p_{t}(MeV/c) | probability   | 100 | 0     | 300 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | gTime_tail      |       | time(ns)     | probability   | 100 | 0     | 2000 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | theta           |       | #theta(rad)  | probability   | 100 | 0     | 1.6   | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | r               |       | radius(mm)   | probability   | 100 | 0     | 350    | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | y               |       |y position(mm)| probability   | 100 | -350   | 350    | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | ox              |       | x(mm) origin | probability   | 100 | 7800|6550 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | oy              |       | y(mm) origin | probability   | 100 | -10|10 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 
	TH1D    | oz              |       | z(mm) origin | probability   | 100 | -5990.5|-5590.5 | 0    | 0     | 1 | 0 | 0  | 0  | 20   | 0   | 

#	TYPE    | NAME            | TITLE | xAxisName         | yAxisName     | BIN | LEFT  | RIGHT | BIN | LEFT  | RIGHT
	TH2D    | thetar          |       | #theta(rad)       | r(mm)         | 240 | 0     | 1.6   | 240 | 0     | 350
	TH2D    | patheta         |       | p_{a}(MeV/c)      | #theta(rad)   | 240 | 0     | 2    | 240 | 0     | 1.6
	TH2D    | oxoz            |       | x(mm) origin      | z(mm) origin  | 240 | 7800| 6550| 240 |-5990.5 |-5590.5
	TH2D    | oxoy            |       | x(mm) origin      | y(mm) origin  | 240 | 7800| 6550| 240 |-10 |10
	TH2D    | par             |       | p_{a}(MeV/c)      | r(mm)         | 240 | 0     | 2    | 240 | 0     | 350 
	TH2D    | pzr             |       | p_{z}(MeV/c)      | r(mm)         | 240 | 0     | 2    | 240 | 0     | 350 
	TH2D    | ptr             |       | p_{t}(MeV/c)      | r(mm)         | 240 | 0     | 2    | 240 | 0     | 350 
	TH2D    | pay             |       | p_{a}(MeV/c)      | y(mm)         | 240 | 0     | 300 | 240 | -350   | 350  
	TH2D    | paox            |       | p_{a}(MeV/c)      | x(mm) origin  | 240 | 0     | 2    | 240 | 7800| 6550 
	TH2D    | pzox            |       | p_{z}(MeV/c)      | x(mm) origin  | 240 | 0     | 2    | 240 | 7800| 6550 
	TH2D    | ptox            |       | p_{t}(MeV/c)      | x(mm) origin  | 240 | 0     | 2    | 240 | 7800| 6550 
	TH2D    | paoy            |       | p_{a}(MeV/c)      | y(mm) origin  | 240 | 0     | 2    | 240 | -10| 10 
	TH2D    | pzoy            |       | p_{z}(MeV/c)      | y(mm) origin  | 240 | 0     | 2    | 240 | -10| 10 
	TH2D    | ptoy            |       | p_{t}(MeV/c)      | y(mm) origin  | 240 | 0     | 2    | 240 | -10| 10 
	TH2D    | paoz            |       | p_{a}(MeV/c)      | z(mm) origin  | 240 | 0     | 2    | 240 | -5990.5|-5590.5 
	TH2D    | pzoz            |       | p_{z}(MeV/c)      | z(mm) origin  | 240 | 0     | 2    | 240 | -5990.5|-5590.5 
	TH2D    | ptoz            |       | p_{t}(MeV/c)      | z(mm) origin  | 240 | 0     | 2    | 240 | -5990.5|-5590.5 
	TH2D    | patheta_tail    |       | p_{a}(MeV/c)      | #theta(rad)   | 240 | 2    | 300 | 240 | 0     | 1.6
	TH2D    | par_tail        |       | p_{a}(MeV/c)      | r(mm)         | 240 | 2    | 300 | 240 | 0     | 350 
	TH2D    | pzr_tail        |       | p_{z}(MeV/c)      | r(mm)         | 240 | 2    | 300 | 240 | 0     | 350 
	TH2D    | ptr_tail        |       | p_{t}(MeV/c)      | r(mm)         | 240 | 2    | 300 | 240 | 0     | 350 
	TH2D    | paox_tail       |       | p_{a}(MeV/c)      | x(mm) origin  | 240 | 2    | 300 | 240 | 7800|6550 
	TH2D    | pzox_tail       |       | p_{z}(MeV/c)      | x(mm) origin  | 240 | 2    | 300 | 240 | 7800|6550 
	TH2D    | ptox_tail       |       | p_{t}(MeV/c)      | x(mm) origin  | 240 | 2    | 300 | 240 | 7800|6550 
	TH2D    | paoy_tail       |       | p_{a}(MeV/c)      | y(mm) origin  | 240 | 2    | 300 | 240 | -10|10 
	TH2D    | pzoy_tail       |       | p_{z}(MeV/c)      | y(mm) origin  | 240 | 2    | 300 | 240 | -10|10 
	TH2D    | ptoy_tail       |       | p_{t}(MeV/c)      | y(mm) origin  | 240 | 2    | 300 | 240 | -10|10 
	TH2D    | paoz_tail       |       | p_{a}(MeV/c)      | z(mm) origin  | 240 | 2    | 300 | 240 | -5990.5|-5590.5 
	TH2D    | pzoz_tail       |       | p_{z}(MeV/c)      | z(mm) origin  | 240 | 2    | 300 | 240 | -5990.5|-5590.5 
	TH2D    | ptoz_tail       |       | p_{t}(MeV/c)      | z(mm) origin  | 240 | 2    | 300 | 240 | -5990.5|-5590.5 
