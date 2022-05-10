
$fn=50;

module hollow_sphere(radius, wall, hole_radius) {
    hole_length = radius * 2;
  
    difference() {
        echo("hole_radius", hole_radius);
        
        // hollowed-out sphere
        echo("radius", radius);
        sphere(r=radius, center=true);
        sphere(r=(radius - wall), center=true);
        
        // holes
        echo("hole_length", hole_length);
        for(rotation = [[0, 0, 0], [90, 0, 0], [0, 90, 0]]) {
            rotate(rotation)
                cylinder(h=hole_length, r=hole_radius, center=true);
        }
    }
}

wall = 1.2;

outer_r = 16;
outer_hole_r = 8;

inner_r = 10;
inner_hole_r = 5;

hollow_sphere(outer_r, wall, outer_hole_r);
translate([0, 0, -(sqrt(outer_r*outer_r - outer_hole_r*outer_hole_r) 
                    - sqrt(inner_r*inner_r - inner_hole_r*inner_hole_r))])
    hollow_sphere(inner_r, wall, inner_hole_r);


