# AQUA
This is the repository for the AQUA Operating system
To run, it's easy. Just copy this huge line, and AQUA will automatically install all dependencies, set up the virtual machine and run it:

`sudo apt-get install git virtualbox make xorriso grub gcc nasm; git clone https://github.com/obiwac/AQUA/;cd AQUA;make build;VBoxManage createvm --name "AQUA OS" --register;VBoxManage modifyvm "AQUA OS" --memory 512;VBoxManage createhd --filename "AQUA Harddrive" --size 2048;VBoxManage storagectl "AQUA OS" --add ide --name "IDE";VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 1 --device 0 --medium aqua/aqua.iso --type dvddrive;VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 0 --device 1 --medium "AQUA Harddrive.vdi" --type hdd;virtualbox --startvm "AQUA OS"`

And if you want to update AQUA, then paste this command:

`git clone https://github.com/obiwac/AQUA/;cd AQUA;VBoxManage createhd --filename "AQUA Harddrive" --size 2048;make iso`
