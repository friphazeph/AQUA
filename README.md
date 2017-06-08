# AQUA
This is the repository for the AQUA Operating system
To run, it's easy. Just do:

`git clone https://github.com/obiwac/AQUA/`

`cd AQUA`

`make build`

`VBoxManage createvm --name "AQUA OS" --register`

`VBoxManage modifyvm "AQUA OS" --memory 512`

`VBoxManage createhd --filename "AQUA Harddrive" --size 2048`

`VBoxManage storagectl "AQUA OS" --add ide --name "IDE"`

`VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 0 --device 0 --medium aqua/aqua.iso --type dvddrive`

`VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 0 --device 1 --medium "AQUA Harddrive.vdi" --type hdd`

`virtualbox --startvm "AQUA OS"`

# OR

If you don't want to copy paste all this manually, line by line:

`sudo apt-get install git virtualbox make xorriso grub gcc nasm; git clone https://github.com/obiwac/AQUA/;cd AQUA;make build;VBoxManage createvm --name "AQUA OS" --register;VBoxManage modifyvm "AQUA OS" --memory 512;VBoxManage createhd --filename "AQUA Harddrive" --size 2048;VBoxManage storagectl "AQUA OS" --add ide --name "IDE";VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 0 --device 0 --medium aqua/aqua.iso --type dvddrive;VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 0 --device 1 --medium "AQUA Harddrive.vdi" --type hdd;virtualbox --startvm "AQUA OS"`
