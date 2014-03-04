#!/usr/bin/env python
from os import getcwd

macro_form = """
/control/verbose 0
/run/verbose 0

/det/add_layer           G4_Cu 150.0 -1.0 -1.0
/det/drift_length        0 mm
/det/target_radius       10.0 cm
/det/world_margin        5 mm
/det/bg_material         G4_Galactic


# new beam info
/beam/gauss/xe   0.01 0.01
/beam/gauss/ye   0.01 0.01

/beam/energy      430 MeV
/beam/m_spread    0.0
/beam/rseed       {seed:d}

/ana/result cu {fname:s} 

/run/initialize

/gun/particle ion
/gun/ion 6 12 6

/run/beamOn 10000
"""

pbs_form = """
#PBS -N {fname}
#PBS -l nodes=1,walltime=72:00:00
#PBS -S /bin/bash
#PBS -q octofarm
#PBS -o {dir}/{fname}.out
#PBS -e {dir}/{fname}.err
cd {home};
./{pname} {dir}/{fname}.mac
"""

project_name = 'blockwin'
n_cpu = 10
cwd = getcwd()
based = cwd[:cwd.find(project_name) + len(project_name)]
exit()
for rseed in range(n_cpu):
    filename = "{0:d}".format(rseed)
    pbs = pbs_form.format(dir = cwd,
                          fname = filename,
                          home = basedir,
                          pname = project_name)
    macro = macro_form.format(seed = rseed, 
                              fname = filename)
    open(filename + '.mac','w').write(macro)
    open(filename + '.pbs','w').write(pbs)

#n_cpu = 10
#t = (44., 88.3, 131.5)
#cwd = getcwd()
#for i in range(len(t)):
#    for j in range(n_cpu):
#        rseed = 20*i + j
#        thickness = t[i]
#        filename = "%dmm_%d" % (thickness, rseed) 
#
#        pbs = pbs_form % (filename,
#                          cwd, filename,
#                          cwd, filename,
#                          cwd, filename)
#        macro = macro_form % (thickness, rseed, filename)
#        
#        #print pbs, macro
#        open(filename + '.mac','w').write(macro)
#        open(filename + '.pbs','w').write(pbs)
        
