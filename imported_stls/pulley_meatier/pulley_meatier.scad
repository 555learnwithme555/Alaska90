
$fs=0.1;
beefyOR=13; // radius of beefy addition
beefyZ=7.0; // height of taper on beefy addition
beefyZE=9.5; // height of end of beef

module beefyPulley() {
  union() {
	import("pulley.stl");
    translate([10,10,0]) 
difference() {
    union() {
        
		// top retainer cone:
        translate([0,0,19]) cylinder(r2=8,r1=5.5,h=3);

        // Outside reinforcing beef:
        difference() {
			union() {
			// main beef
           	cylinder(r=beefyOR,h=beefyZ+0.1);
        // Bottom roll-on cone:
        translate([0,0,beefyZ]) cylinder(r1=beefyOR,r2=6,h=beefyZE-beefyZ);
			}
			union() {
			// hole for bolt head
			 translate([0,0,4]) rotate([-90,0,0]) cylinder(r=3.5,h=15);

			// big hole in middle of beef, for nut trap
          	 translate([0,0,-0.5]) cylinder(r=8,h=8);
			}
        }
    }
    cylinder(r=2.5,h=30); // axle hole
}
  }
}

beefyPulley();
