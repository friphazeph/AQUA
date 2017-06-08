# AQUA
This is the repository for the AQUA Operating system
To run, it's easy. Just do:

`git clone https://github.com/obiwac/AQUA/`

`cd AQUA`

`VBoxManage createvm --name "AQUA OS" --register`

`VBoxManage modifyvm "AQUA OS" --memory 512`

`VBoxManage createhd --filename "AQUA Harddrive" --size 2048`

`VBoxManage storageattach "AQUA OS" --storagectl "IDE" --port 0 --device 1 --medium "AQUA Harddrive.vdi" --type hdd`

`make iso`
