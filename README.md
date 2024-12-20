# Neutron detector Geant4 code

![Alt text](https://github.com/dinAlt220/Ndet/image.png)

(Magnetic field in shared mem, test on Debian)

## How to use

1. Make sure Data.txt in directory has a correct path `ifstream file("/home/alion/simulation/BMN/Ndet/shared_mem/Data.txt")` in `load_field.cc`
Then type in cmd
2. `cd path_to_Ndet/Ndet/shared_mem`
3. `g++ -o get get_field.cc -lrt` - script to extract data field from the field data array (test only, works only after `./load` step 6)
4. `g++ -o delete delete.cc -lrt` - scrip to clear shared mem
5. `g++ -o load load_field.cc -lrt` - scrip to load magnetic field data to shared mem
6. `./load` - at this step field data will be loaded to shared mem, will take some time (~ 10 minutes)
6 step field data is in your shared mem, if want to clear it, type `./delete`

Now to Geant4 workspace

7. `cd path_to_Ndet/Ndet/build`, if no `build` directory - create it
8. `source path_to_geant4_install_dir/bin/geant4.sh` - export geant4 libraries
9. `cmake -DGeant4_DIR=/path_to_geant4_install_dir path_to_Ndet/Ndet/`
10. `./Ndet` - run the simulation

At the moment there is 5 GeV proton beam that hit a liquid hydrogen target placed in the center of the field. Visulization file is created in the `build` directory after simulation end.

Change `bool Interpolation = false;` value to eather use interpolation or no (3x3x3 linear interpolation, computation time increses)