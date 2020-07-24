uniform bool u_draw_wireframe;
uniform vec3 u_selectorcolor;
uniform bool u_selectorpass;
uniform bool u_isselected;

bool life_defaults() {
    if(u_draw_wireframe) {
        color = vec4(1,1,1,1);
        return true;
    }
    if(u_isselected) {
        color = vec4(1,1,1,1); // TODO: We should have another selected color
        return true;
    }
    if(u_selectorpass) {
        color = vec4(u_selectorcolor,1.0);
        return true;
    }
    return false;
}