# Ordering instructions

If you want to order the PCB from PCBWay, here's what files to use:


* The file `gerbers_bom_and_cpl.zip` in `sotpotatis_pomodoro_timer.kicad_pcb` can be uploaded when requested to provide files related to the PCB (*not the assembly service*).

* The `.zip` file just mentioned includes both the BOM and CPL (i.e. the files required for the assembly service), but it turns out you need to supply them separately (PCBWay's upload form doesn't magically detect that you have provided a .zip file with both production and assembly files). Thus, *for the assembly service*, you should also upload `PCBWay_bom.csv` and `PCBWay_positions.csv` found in the `sotpotatis_pomodoro_timer.kicad_pcb` directory (or in the `.zip` file, it doesn't matter, they are the same files).


> For a visual image, see `sample_pcbway_order_information.png`. It shows a brief outlines of the files and settings I used when ordering the PCB.
