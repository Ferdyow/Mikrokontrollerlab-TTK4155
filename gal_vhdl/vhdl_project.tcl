
########## Tcl recorder starts at 09/06/17 12:58:01 ##########

set version "2.0"
set proj_dir "C:/Users/tobib/byggern"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 12:58:01 ###########


########## Tcl recorder starts at 09/06/17 16:01:36 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:01:36 ###########


########## Tcl recorder starts at 09/06/17 16:02:19 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:02:19 ###########


########## Tcl recorder starts at 09/06/17 16:02:30 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:02:30 ###########


########## Tcl recorder starts at 09/06/17 16:02:42 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:02:42 ###########


########## Tcl recorder starts at 09/06/17 16:03:39 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:03:39 ###########


########## Tcl recorder starts at 09/06/17 16:04:15 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:04:15 ###########


########## Tcl recorder starts at 09/06/17 16:04:18 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:04:18 ###########


########## Tcl recorder starts at 09/06/17 16:04:43 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:04:43 ###########


########## Tcl recorder starts at 09/06/17 16:05:09 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:05:09 ###########


########## Tcl recorder starts at 09/06/17 16:05:17 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:05:18 ###########


########## Tcl recorder starts at 09/06/17 16:56:11 ##########

set version "2.0"
set proj_dir "C:/Users/tobib/byggern/gal_vhdl"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:56:11 ###########


########## Tcl recorder starts at 09/06/17 16:56:24 ##########

# Commands to make the Process: 
# Create Fuse Map
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj vhdl_project -if vhdl_project.jed -j2s -log vhdl_project.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 16:56:24 ###########


########## Tcl recorder starts at 09/06/17 17:11:51 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:11:51 ###########


########## Tcl recorder starts at 09/06/17 17:16:03 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:16:03 ###########


########## Tcl recorder starts at 09/06/17 17:16:05 ##########

# Commands to make the Process: 
# Chip Report
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:16:05 ###########


########## Tcl recorder starts at 09/06/17 17:25:57 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:25:57 ###########


########## Tcl recorder starts at 09/06/17 17:26:03 ##########

# Commands to make the Process: 
# Create Fuse Map
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj vhdl_project -if vhdl_project.jed -j2s -log vhdl_project.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:26:03 ###########


########## Tcl recorder starts at 09/06/17 17:30:42 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:30:42 ###########


########## Tcl recorder starts at 09/06/17 17:31:21 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:31:21 ###########


########## Tcl recorder starts at 09/06/17 17:31:28 ##########

# Commands to make the Process: 
# Create Fuse Map
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd
if [runCmd "\"$cpld_bin/edif2blf\" -edf \"GAL_addrSelect.edi\" -out \"GAL_addrSelect.bl0\" -err automake.err -log \"GAL_addrSelect.log\" -prj vhdl_project -lib \"$install_dir/ispcpld/dat/mach.edn\" -cvt YES -net_Vcc VCC -net_GND GND -nbx -dse -tlw"] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" \"GAL_addrSelect.bl0\" -red bypin choose -collapse -pterms 8 -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblflink\" \"GAL_addrSelect.bl1\" -o \"vhdl_project.bl2\" -omod GAL_addrSelect -family -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/iblifopt\" vhdl_project.bl2 -red bypin choose -sweep -collapse all -pterms 8 -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/idiofft\" vhdl_project.bl3 -pla -o vhdl_project.tt2 -dev p16v8 -define N -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fit\" vhdl_project.tt2 -dev p16v8 -str -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/fuseasm\" vhdl_project.tt3 -dev p16v8 -o vhdl_project.jed -ivec NoInput.tmv -rep vhdl_project.rpt -doc brief -con ptblown -for brief -err automake.err "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
if [runCmd "\"$cpld_bin/synsvf\" -exe \"$install_dir/ispvmsystem/ispufw\" -prj vhdl_project -if vhdl_project.jed -j2s -log vhdl_project.svl "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/06/17 17:31:28 ###########


########## Tcl recorder starts at 09/20/17 11:21:21 ##########

set version "2.0"
set proj_dir "C:/Users/ferdyow/Desktop/byggern/gal_vhdl"
cd $proj_dir

# Get directory paths
set pver $version
regsub -all {\.} $pver {_} pver
set lscfile "lsc_"
append lscfile $pver ".ini"
set lsvini_dir [lindex [array get env LSC_INI_PATH] 1]
set lsvini_path [file join $lsvini_dir $lscfile]
if {[catch {set fid [open $lsvini_path]} msg]} {
	 puts "File Open Error: $lsvini_path"
	 return false
} else {set data [read $fid]; close $fid }
foreach line [split $data '\n'] { 
	set lline [string tolower $line]
	set lline [string trim $lline]
	if {[string compare $lline "\[paths\]"] == 0} { set path 1; continue}
	if {$path && [regexp {^\[} $lline]} {set path 0; break}
	if {$path && [regexp {^bin} $lline]} {set cpld_bin $line; continue}
	if {$path && [regexp {^fpgapath} $lline]} {set fpga_dir $line; continue}
	if {$path && [regexp {^fpgabinpath} $lline]} {set fpga_bin $line}}

set cpld_bin [string range $cpld_bin [expr [string first "=" $cpld_bin]+1] end]
regsub -all "\"" $cpld_bin "" cpld_bin
set cpld_bin [file join $cpld_bin]
set install_dir [string range $cpld_bin 0 [expr [string first "ispcpld" $cpld_bin]-2]]
regsub -all "\"" $install_dir "" install_dir
set install_dir [file join $install_dir]
set fpga_dir [string range $fpga_dir [expr [string first "=" $fpga_dir]+1] end]
regsub -all "\"" $fpga_dir "" fpga_dir
set fpga_dir [file join $fpga_dir]
set fpga_bin [string range $fpga_bin [expr [string first "=" $fpga_bin]+1] end]
regsub -all "\"" $fpga_bin "" fpga_bin
set fpga_bin [file join $fpga_bin]

if {[string match "*$fpga_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$fpga_bin;$env(PATH)" }

if {[string match "*$cpld_bin;*" $env(PATH)] == 0 } {
   set env(PATH) "$cpld_bin;$env(PATH)" }

lappend auto_path [file join $install_dir "ispcpld" "tcltk" "lib" "ispwidget" "runproc"]
package require runcmd

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/17 11:21:21 ###########


########## Tcl recorder starts at 09/20/17 11:21:40 ##########

# Commands to make the Process: 
# Synplify Synthesize VHDL File
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd

########## Tcl recorder end at 09/20/17 11:21:40 ###########


########## Tcl recorder starts at 09/20/17 11:22:00 ##########

# Commands to make the Process: 
# Synplify Synthesize VHDL File
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd

########## Tcl recorder end at 09/20/17 11:22:00 ###########


########## Tcl recorder starts at 09/20/17 11:23:22 ##########

# Commands to make the Process: 
# Synplify Synthesize VHDL File
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd

########## Tcl recorder end at 09/20/17 11:23:22 ###########


########## Tcl recorder starts at 09/20/17 11:25:35 ##########

# Commands to make the Process: 
# Synplify Synthesize VHDL File
if [catch {open GAL_addrSelect.cmd w} rspFile] {
	puts stderr "Cannot create response file GAL_addrSelect.cmd: $rspFile"
} else {
	puts $rspFile "STYFILENAME: vhdl_project.sty
PROJECT: GAL_addrSelect
WORKING_PATH: \"$proj_dir\"
MODULE: GAL_addrSelect
VHDL_FILE_LIST: gal.vhd
OUTPUT_FILE_NAME: GAL_addrSelect
SUFFIX_NAME: edi
"
	close $rspFile
}
if [runCmd "\"$cpld_bin/Synpwrap\" -e GAL_addrSelect -target ispGAL -pro "] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}
file delete GAL_addrSelect.cmd

########## Tcl recorder end at 09/20/17 11:25:35 ###########


########## Tcl recorder starts at 09/20/17 11:28:49 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/17 11:28:49 ###########


########## Tcl recorder starts at 09/20/17 11:28:55 ##########

# Commands to make the Process: 
# Hierarchy
if [runCmd "\"$cpld_bin/vhd2jhd\" \"gal.vhd\" -o \"gal.jhd\" -m \"$install_dir/ispcpld/generic/lib/vhd/location.map\" -p \"$install_dir/ispcpld/generic/lib\""] {
	return
} else {
	vwait done
	if [checkResult $done] {
		return
	}
}

########## Tcl recorder end at 09/20/17 11:28:55 ###########

