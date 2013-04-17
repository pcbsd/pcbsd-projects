This directory just provides a couple shell scripts for packaging up the 
EasyPBI sources for distibution.

The step to complete this process are:
1) Edit step1.sh to make sure that the LOCALSRCDIR variable is pointing 
to your local directory of EasyPBI sources

2) Edit step2.sh and set the proper VERSION for the distibution
   (it should match the current program version in the sources)

3) Run step1.sh and follow any directions ("sh step1.sh")
4) Run step2.sh and follow any directions ("sh step2.sh")

This should generate a easypbi-<version>.tar.gz file that can be 
distributed as necessary.

~~Ken Moore~~
ken@pcbsd.org
1/17/2013
