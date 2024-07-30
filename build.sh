source /home/lev/root/root_v6.26.10/root/bin/thisroot.sh

# HepMC3
CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/home/lev/HepMC3/hepmc3-install/share/HepMC3/cmake/

# TBB
CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/home/lev/oneTBB/install/lib/cmake/TBB/

export CMAKE_PREFIX_PATH

cmake -DPYTHIA8_ROOT_DIR=/home/lev/pythia8310 ..