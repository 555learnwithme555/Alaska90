//
// Mendel90
//
// GNU GPL v2
// nop.head@gmail.com
// hydraraptor.blogspot.com
//
// Configuration file
//
echo("Lawlor90:");

Z_bearings = LM16UU; // 16mm == 5/8" foosball shaft, and same diameter as 30x40T CNC cutter
Y_bearings = LM16UU;
X_bearings = LM16UU;

X_motor = NEMA17;
Y_motor = NEMA17;
Z_motor = NEMA17;

hot_end = makerbotMk6;

X_travel = 305; // 12" (trimmed to get total width below 24")
Y_travel = 590; // 24" (trimmed to get total base length below 48")
Z_travel = 390; // 15" (trimmed to get total Z under 24")

bed_depth = 490; // trimmed to leave some meat over gantry
bed_width = 310;
bed_pillars = QIx13_pillar; // M3x20_pillar;
bed_glass = glass2;
bed_thickness = pcb_thickness + sheet_thickness(bed_glass);    // PCB heater plus glass sheet
bed_holes = [bed_width-5, bed_depth-5];

base = MDF18;
base_corners = 0;

frame = MDF18;
frame_corners = 0;
frame_nuts = false;

case_fan = fan80x38;
psu = ALPINE500;
//psu = KY240W;
controller = Melzi;
//controller = Sanguinololu;
spool = spool_300x85;
bottom_limit_switch = true;
top_limit_switch = true;

single_piece_frame = true;
stays_from_window = false;

Y_carriage = MDF10;

X_belt = T5x6;
Y_belt = T5x6;

motor_shaft = 5;

Z_screw_dia = 12.6;            // Studding for Z axis: 1/2" ACME rod, 10tpi

Y_carriage_depth = bed_holes[1] + 8;
Y_carriage_width = bed_holes[0] + 8;

Z_nut_radius = 11; // M8_nut_radius;
Z_nut_depth = 12.25; // M8_nut_depth;
// Z_nut = M8_nut;

//
// Default screw use where size doesn't matter
//
cap_screw = QI_hex_screw;
hex_screw = QI_hex_screw;
//
// Screw for the frame and base
//
frame_soft_screw = QI_hex_screw; // No6_screw;               // Used when sheet material is soft, e.g. wood
frame_thin_screw = QI_hex_screw; // M4_cap_screw;            // Used with nuts when sheets are thin
frame_thick_screw = QI_hex_screw; // M4_pan_screw;           // Used with tapped holes when sheets are thick and hard, e.g. plastic or metal
//
// Feature sizes
//
default_wall = 5;
thick_wall = 6;
