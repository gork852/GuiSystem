#version 420

in vec4 box_point_pos;
in vec4 box_point_cords;
in vec4 box_point_color;
in vec4 box_boarder_amt;
in vec4 box_boarder_color;

out vec4 current_cords;
out vec4 current_color;
out vec4 current_boarder_amt;
out vec4 current_boarder_color;


void main() {
  gl_Position = box_point_pos*2-vec4(1,1,0,1);
  
  current_cords = box_point_cords;
  current_color = box_point_color;
  current_boarder_amt = box_boarder_amt;
  current_boarder_color = box_boarder_color;
}