
module roundedCube(w,h,d,dia) {
    hull()
    {
        sphere(d=dia);
        translate([0,h,d])sphere(d=dia);
        translate([w,0,d])sphere(d=dia);
        translate([w,h,0])sphere(d=dia);
        translate([w,h,d])sphere(d=dia);
        translate([0,0,d])sphere(d=dia);
        translate([w,0,0])sphere(d=dia);
        translate([0,h,0])sphere(d=dia);
    }
}

module arrow() {
    base=4;
    peak=base/2;
    thickness=0.5;
    union()
    {
        cube([base/2, base/2, thickness], center=true);
        translate([-base/2, peak/2, -thickness/2])
            polyhedron // triangle
            (
                points = 
                [
                    [0,0,0], 
                    [base, 0, 0], 
                    [base/2, peak, 0],
                    [0,0,thickness], 
                    [base, 0, thickness], 
                    [base/2, peak, thickness]
                ],
                faces = 
                [
                    [2, 1, 0], 
                    [0, 1, 4, 3],
                    [1, 2, 5, 4],
                    [5, 2, 0, 3],
                    [3, 4, 5]
                ]
            );
    }
}

module extrudedTriangle(base, thickness) {
    peak=base/2;
    rotate([0.0, 0.0, 45.0])
    polyhedron // triangle
    (
        points = 
        [
            [0,0,0], 
            [base, 0, 0], 
            [base/2, peak, 0],
            [0,0,thickness], 
            [base, 0, thickness], 
            [base/2, peak, thickness]
        ],
        faces = 
        [
            [2, 1, 0], 
            [0, 1, 4, 3],
            [1, 2, 5, 4],
            [5, 2, 0, 3],
            [3, 4, 5]
        ]
    );
}

module hole(x, y, z) {
    translate([x, y, z])
    {
        color("Red", 1.0)
        rotate([0, 0, 180.0])
        cylinder(h=3, r=0.4, center=true);
    }
}