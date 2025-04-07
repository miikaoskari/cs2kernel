# cs2kernel

kernel level driver to read cs2 memory

## loading the driver

### disable checks

`bcdedit.exe /set testsigning on`

`bcdedit.exe /set nointegritychecks on`

### create the driver

`sc.exe create driver type= kernel binPath= C:\absolute\path\to\driver.sys`

### start the driver

`sc.exe start driver`

