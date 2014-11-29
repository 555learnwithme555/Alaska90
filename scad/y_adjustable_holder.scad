//
// Mendel90
//
// GNU GPL v2
// lawlor@alaska.edu
//
// Hold a 608 bearing 
//
include <conf/config.scad>

adjust_nut=QI_nut;
adjust_nutR=nut_radius(adjust_nut)+0.2;
adjust_shaftR=0.25*25.4*1.05/2;

sidewall=3; // main body walls
wall=4; // default wall thickness
bearingIR=8/2;  // inside radius of bearing
bearingOR=22/2;  // outside radius of bearing
bearingH=9; // Y height of bearing slot
insideX=40; // X width of interior hanger

module y_adjustable_holder(){
    stl("y_adjustable_holder");

	translate([0,0,0]) rotate([90,0,0]) difference() {

// Body:
		translate([0,0,-(wall+bearingH+wall)/2])
		rounded_rectangle([insideX+2*wall,2*bearingOR,wall+bearingH+wall],bearingOR*0.5,false);

// Main slot:	
			rotate([90,0,0])
		translate([1.5*wall,0,-(wall+bearingH+wall)])
			rounded_rectangle([insideX+2*wall,bearingH,8*bearingIR],bearingIR,false); 

// Bearing-side holes
		translate([(insideX+2*wall)/2-bearingOR,0,-(1.5*wall+bearingH)])
union() {
		cylinder(r=bearingIR,h=4*wall+bearingH); // axle hole
		// #cylinder(r=bearingOR,h=bearingH); // bearing
}

// Hole for nut
   translate([0,0,0]) rotate([-90,0,90]) union() {
			cylinder(h = insideX/2, r=adjust_nutR, $fn=6, center=false);
			cylinder(h = insideX/2+3*wall, r=adjust_shaftR, center=false);
	}

    }
}


baseY=1.2*(wall+bearingH+wall); // Y height of slot inside base
baseLX=-30; // X location of walls toward mount hole
baseRX=20; // X length of walls toward slot

bearingTop=28; // Z height of top of bearing
holeZ=bearingTop-bearingOR; // centerline of mounting hole


module y_adjustable_base(){
    stl("y_adjustable_base");

translate([-insideX-2*wall,0,holeZ-bearingOR])
difference() {
	// side walls
	union() {
		rotate([90,0,0])
		 linear_extrude(height = baseY+2*sidewall, center = true)
        polygon(points = [[baseLX,-holeZ+wall],[baseLX,-holeZ], 
			[baseRX, -holeZ], [baseRX, -holeZ+sidewall], 
			[+wall/2, bearingOR],[-wall/2, bearingOR]]);

	}
	// center slot clearance
	translate([(insideX+wall)/2,0,-holeZ+sidewall]) {
		rounded_rectangle([insideX,baseY,2*holeZ],3,false); 
	}
	// mount hole clearance
	translate([-(insideX+wall)/2,0,-holeZ+wall]) {
		rounded_rectangle([insideX,baseY,2*holeZ],3,false); 
	}
	// mount hole
translate([baseLX+3*adjust_shaftR,0,-holeZ-eta])
	cylinder(h = 2*wall, r=adjust_shaftR, center=false);
	
	// adjust hole
translate([-wall/2-eta,0,0])
rotate([0,90,0])
	cylinder(h = 2*wall, r=adjust_shaftR, center=false);
}


}


y_adjustable_holder();
y_adjustable_base();
