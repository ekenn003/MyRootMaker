# MyRootMaker
MyRootMaker ntuplizer 

https://twiki.cern.ch/twiki/bin/view/Sandbox/MyRootMakerFrom74XTo76X

Recipe:

    cmsrel CMSSW_7_6_3_patch2
    cd CMSSW_7_6_3_patch2/src/
    cmsenv
    git cms-init

    git clone https://github.com/ekenn003/PFIsolation.git
    git clone https://github.com/ekenn003/MyRootMaker.git

    scram b -j 8
    
    cmsRun MyRootMaker/MyRootMaker/RootTreeMC.py (monte carlo)  
    cmsRun MyRootMaker/MyRootMaker/RootTreeDA.py (data)
