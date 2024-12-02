echo(version=version());
$fn = $preview ? 32 : 64;

module reverse(x=0)
{
    translate([x, 0, 0])
    rotate([0, 180, 0])
    children();
}


include <util_primitives.scad>





union()
{
BasePCB();
translate([18, 80, -1])
reverse(18)
    ESP();

translate([31, 16, 0])
reverse()
    Buzzer();

translate([1.5, 91, 0])
reverse(2.7)
    PinSet(5, [0,-1])
        PinM();

translate([1.5, 50.5, 2])
    PinSet(16, [0,-1])
        PinF();
}



module BasePCB()
{

    pcb = [65, 100, 2];
    hole_radius = 3.4 / 2;
    hole_x_off = 4+hole_radius;
    hole_y_off = 2+hole_radius;

// BASE PCB
    difference()
    {
        color("green")
            CubeRoundedXY(pcb, 5);

        union()
        {
            translate([hole_x_off, hole_y_off, -1])
                cylinder(4, r=hole_radius);
            translate([hole_x_off, pcb.y - hole_y_off, -1])
                cylinder(4, r=hole_radius);
            translate([pcb.x - hole_x_off, hole_y_off, -1])
                cylinder(4, r=hole_radius);
            translate([pcb.x - hole_x_off, pcb.y - hole_y_off, -1])
                cylinder(4, r=hole_radius);
        }
    }
}
module ESP()
{
    pcb = [18, 26, 1];
    case = [16, 18, 2.5];
    
    color("black")
        cube(pcb);
    
    color("grey")
    translate([1, 1, 1])
        cube(case);
}


module Buzzer()
{
    union()
    {
        difference()
        {
            color("black")
                cylinder(6, r = 4.5);
            
            translate([0, 0, 7-2])
                cylinder(2, r=1);
        }
    }
}





module PinSet(num, ort)
{
   for (i = [0 : num-1])
     translate([ort.x*2.7*i, ort.y*2.7*i, 0 ])
        children();
}


module PinM()
{
    off = 2.7 / 2 - 0.5;
    
    color("grey")
    translate([off, off, -3])
        cube([1, 1, 11.5]);

    color("black")
        CubeRoundedXY(2.7, 0.5);
}
module PinF()
{
    function off(s=1, c=false) = (2.7 / 2 - (c ? 0 : s / 2));
    
    color("grey")
    translate([off(), off(), -3.5])
        cube([1, 1, 3.5]);

    difference()
    {
        color("grey")
            cube([2.7, 2.7, 9]);
        union()
        {
            translate([off(), off(), 1])
                cube([1, 1, 9]);
            translate([off(0, true), off(0, true), 9])
            union()
            {
                translate([0, 0, 0.999])
                    cube([2.2, 2.2, 2], true);
                reverse()
                    Pyramid(2.2, 2.2, 2);
            }
        }
    }
}