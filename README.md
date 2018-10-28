# cse 221, systems measurement project
http://9front.org/

## useful manual:
http://www.covingtoninnovations.com/michael/newbie-guide.pdf

## Installation
### Qemu
install:
```bash
qemu-system-x86_64 -cpu host -enable-kvm -m 1024 -net nic,model=virtio,macaddr=52:54:00:00:EE:03 -net user -show-cursor -device virtio-scsi-pci,id=scsi -drive if=none,id=vd0,file=9front.qcow2.img -device scsi-hd,drive=vd0 -drive if=none,id=vd1,file=9front.iso -device scsi-cd,drive=vd1,bootindex=0,
```
Install according to the instructions at fqa.9front.org/fqa4.html#4.3.5 (just do `inst/start`)

Then boot with:
```bash
qemu-system-x86_64 -cpu host -enable-kvm -m 1024 \
    -net nic,model=virtio,macaddr=52:54:00:00:EE:03
    -net user,hostfwd=tcp::17019-:17019,hostfwd=tcp::17020-:17020,hostfwd=tcp::12567-:567 \
    -device virtio-scsi-pci,id=scsi \
    -drive if=none,id=vd0,file=9front.qcow2.img -device scsi-hd,drive=vd0 \
    -show-cursor
```

##
