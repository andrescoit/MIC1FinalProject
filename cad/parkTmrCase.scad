raw_dim=[80,85,21];
batt_height=18;
shell_offset=0;
bot_dim=[raw_dim[0],raw_dim[1],raw_dim[2]/2-shell_offset];
top_dim=[raw_dim[0],raw_dim[1],raw_dim[2]/2+shell_offset];
lcd_pos=[5,6];
lcd_dim=[71,24];
lcd_r=5;
hdrs_pos=[raw_dim[0]-17,44];
hdrs_dim=[10,5];
case_thk=6;

parkTmrCaseBot(dim=bot_dim,batt_height=batt_height,thickness=case_thk);
translate([0,-top_dim[1]-3*6,0]) parkTmrCaseTop(dim=top_dim,batt_height=batt_height,lcd_pos=lcd_pos,lcd_dim=lcd_dim,lcd_bevel_r=lcd_r,thickness=case_thk,hdrs_pos=hdrs_pos,hdrs_dim=hdrs_dim);
//parkTmrCaseTop(dim=top_dim,batt_height=batt_height,lcd_pos=lcd_pos,lcd_dim=lcd_dim,lcd_bevel_r=lcd_r,thickness=case_thk,hdrs_pos=hdrs_pos,hdrs_dim=hdrs_dim);
module parkTmrCaseBot(dim=[80,60,20],batt_height=20,thickness=6,lip_h=4) {
    difference(){
        bevelCube(dim,thickness);
        translate([0,0,0]) cube(dim);
        translate([-thickness,-thickness,dim[2]]) cube([dim[0]+2*thickness,dim[1]+2*thickness,thickness]);
    }
    translate([thickness/3,thickness/3,dim[2]]) roundedLip([dim[0]-2/3*thickness,dim[1]-2/3*thickness],thickness=thickness,lip_th=thickness/4,lip_h=lip_h);
    translate([10/2,batt_height,0]) cube([dim[0]-10,thickness,dim[2]]);
}
module parkTmrCaseTop(dim=[80,60,20],batt_height=20,thickness=6,lip_h=4,lcd_pos=[10,10],lcd_dim=[60,20],lcd_bevel_r=3,hdrs_pos=[80-17,14],hdrs_dim=[10,5]) {
    difference(){
        bevelCube(dim,thickness);
        translate([0,0,0]) cube(dim);
        translate([-thickness,-thickness,dim[2]]) cube([dim[0]+2*thickness,dim[1]+2*thickness,thickness]);
        translate([thickness/3,thickness/3,dim[2]-lip_h]) roundedLip([dim[0]-2/3*thickness,dim[1]-2/3*thickness],thickness=thickness,lip_th=thickness/4,lip_h=lip_h);
        translate([lcd_pos[0],lcd_pos[1],-thickness]) cube([lcd_dim[0],lcd_dim[1],thickness]);
        translate([hdrs_pos[0],hdrs_pos[1],-thickness]) cube([hdrs_dim[0],hdrs_dim[1],thickness]);
    }
    translate([10/2,dim[1]-batt_height-thickness,0]) cube([dim[0]-10,thickness,dim[2]]);
    translate([lcd_pos[0],lcd_pos[1],-thickness]) translate([0,lcd_dim[1],thickness]) rotate([180,0,0]) roundedInrBvl([lcd_dim[0],lcd_dim[1],thickness],r=lcd_bevel_r);
}
module bevelCube(dim=[10,10,10],thickness=6) {
    minkowski() {
        sphere(center=true,r=thickness,$fn=50);
        cube(dim);
    }
}
//parkingTimerCaseBot();
module roundedLip(dim=[80,60],lip_h=6/2,thickness=6,lip_th=1) {
    difference() {
        minkowski() {
            cube([dim[0],dim[1],lip_h/2]);
            cylinder(h=lip_h/2,r=thickness,$fn=50);
        }
        translate([lip_th,lip_th,0]) minkowski() {
            cube([dim[0]-lip_th*2,dim[1]-lip_th*2,lip_h]);
            cylinder(h=lip_h,r=thickness,$fn=50);
        }
    }
}
module roundedInrBvl(dim=[75,40,10],r=3) {
    intersection() {
        minkowski() {
            difference() {
                translate([-r,-r,0]) cube([dim[0]+2*r,dim[1]+2*r,dim[2]-r]);
                cube([dim[0],dim[1],dim[2]-r]);
            }
        sphere(r=r,$fn=50);
        }
        cube(dim);
    }
}