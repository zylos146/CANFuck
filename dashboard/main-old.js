/*   
     Simple oscilloscope trace plot using WebGL
     Copyright (c) Jeremy P Bentham 2021. See http://iosoft.blog for details

     Licensed under the Apache License, Version 2.0 (the "License");
     you may not use this file except in compliance with the License.
     You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0

     Unless required by applicable law or agreed to in writing, software
     distributed under the License is distributed on an "AS IS" BASIS,
     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
     See the License for the specific language governing permissions and
     limitations under the License.

     v0.17 JPB 13/1/21  Removed duplicate init_graph
*/
// TODO - Update notice to reflect the heavy modifications done here

const NORM_XMIN = -1.0, NORM_XMAX = 1.0, NORM_YMIN = -1.0, NORM_YMAX = 1.0;
const XMARGIN = 20, YMARGIN = 90, MIN_CHANS = 1, MAX_CHANS = 16, NCHANS = 2;
var data_srce = sim_srce = "sim", fifo_srce = "fifo";
var canvas = document.getElementById('graph_canvas');
var gl = canvas.getContext('webgl2');
var clear_colour = [0.82, 0.87, 0.82, 1.0];
var trace_colours = [[0.6, 0.7, 0.6, 1],
colr(0x000000), colr(0x800000), colr(0xff0000), colr(0xff9900),
colr(0xffff00), colr(0x00ff00), colr(0x0000ff), colr(0xff00ff),
colr(0x969696), colr(0xffffcc), colr(0x000000), colr(0x800000),
colr(0xff0000), colr(0xff9900), colr(0xffff00), colr(0x00ff00)];

var program, running = false, trace_ymax = 2.0, num_chans = NCHANS;
var frag_code, vert_code;

//Fragment shader for WebGL 2.0 and 1.0 (GLES 3.0 and 2.0)
  frag_code = `#version 300 es
                precision mediump float;
                in vec4 v_colour;
                out vec4 o_colour;
                void main() {
                    o_colour = v_colour;
                }`;

// Vertex shader for WebGL 2.0 and 1.0
  vert_code = `#version 300 es
                in vec3 a_coords;
                out vec4 v_colour;
                `;
vert_code +=
  `#define MAX_CHANS ${MAX_CHANS}
            uniform vec4 u_colours[MAX_CHANS];
            void main(void) {
               int zint = int(a_coords.z);
               gl_Position = vec4(a_coords.x, a_coords.y, 0, 1);
               v_colour = u_colours[zint];
            }`;

const MS_PER_FRAME = 33.3;
const PX_PER_DIV = 32;
const MS_PER_DIV = 10 * MS_PER_FRAME; 
const GRID_COLOR = 9;

// Start creating display
function start_graph() {
  resize_canvas();
  var sel = document.getElementById("sel_nchans");
  for (var n = MIN_CHANS; n <= MAX_CHANS; n++)
    sel.options.add(new Option(n + " channel" + (n > 1 ? "s" : ""), value = n));
  sel.selectedIndex = NCHANS - 1;
  var sel = document.getElementById("sel_srce");
  sel.options.add(new Option("simulated", value = sim_srce));
  sel.options.add(new Option("from " + fifo_srce, value = fifo_srce));
  try {
    init_graph();
  } catch (e) {
    alert("Error: " + e);
  }
  window.addEventListener("resize", resize_canvas);

  init_scale_offset(trace_ymax);

  let msTime = 0;
  let data = [];

  setInterval(() => {
    const xDivisions = Math.ceil(gl.drawingBufferWidth / PX_PER_DIV);
    const yDivisions = Math.floor(gl.drawingBufferHeight / PX_PER_DIV);

    gl.clearColor(...clear_colour);
    
    grid_vertices = []
    trace_vertices = []
    draw_grid(grid_vertices, msTime);

    const value = Math.sin(msTime / (2 * Math.PI / 2000));
    data.push([msTime, value]);

    vals.push((Math.sin(i / 10.0 - Math.PI / 2) + 1) *
      trace_ymax / (2.0 + i / 100.0));
    draw_trace(trace_vertices, NORM_XMIN, NORM_XMAX, data, 1);

    redraw_data();
    redraw_graph();

    msTime += MS_PER_FRAME;
  }, MS_PER_FRAME);

  // Cleanup data that falls out of two width ranges
  setInterval(() => {
    const xDivisions = Math.ceil(gl.drawingBufferWidth / PX_PER_DIV);

    let toDelete = 0;
    for (let i = 0; i < data.length; i++) {
      const [_msTime, value] = data[i];
      const timeDiff = msTime - _msTime
      if (timeDiff > MS_PER_DIV * 2 * xDivisions) {
        toDelete += 1;
      } else {
        break;
      }
    }

    data.splice(0, toDelete);
  }, 5000);
}

// Initialise graph
function init_graph() {
  var vertex_buffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
  var frag_shader = compile_shader(gl.FRAGMENT_SHADER, frag_code);
  var vert_shader = compile_shader(gl.VERTEX_SHADER, vert_code);
  program = gl.createProgram();
  gl.attachShader(program, vert_shader);
  gl.attachShader(program, frag_shader);
  gl.linkProgram(program);
  gl.useProgram(program);
  gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
  var coord = gl.getAttribLocation(program, "a_coords");
  gl.vertexAttribPointer(coord, 3, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(coord);
  var clrs = gl.getUniformLocation(program, 'u_colours');
  gl.uniform4fv(clrs, new Float32Array(trace_colours.flat()));
  disp_status(location.host ? "Loaded from " + location.host :
    "Loaded from filesystem, no data available");
}

// Compile a shader
function compile_shader(typ, source) {
  var s = gl.createShader(typ);
  gl.shaderSource(s, source);
  gl.compileShader(s);
  if (!gl.getShaderParameter(s, gl.COMPILE_STATUS))
    throw "Could not compile " +
    (typ == gl.VERTEX_SHADER ? "vertex" : "fragment") +
    " shader:\n\n" + gl.getShaderInfoLog(s);
  return (s);
}

// Handle 'single' button press
function run_single(btn) {
  if (running)
    run_stop(elem('run_stop_btn'));
  var vals = get_data(data_srce);
}
// Handle 'run/stop' button press
function run_stop(btn) {
  running = !running;
  btn.innerText = running ? "Stop" : "Run";
  if (running)
    window.requestAnimationFrame(redraw_graph);
}

// Update the status display
function disp_status(s) {
  elem('status').innerHTML = s.trim();
}

// Handle resizing of the canvas
function resize_canvas() {
  canvas.width = window.innerWidth - XMARGIN;
  canvas.height = window.innerHeight - YMARGIN;
  redraw_graph();
}

// Change number of channels
function sel_nchans() {
  var sel = document.getElementById("sel_nchans");
  num_chans = sel.options[sel.selectedIndex].value;
  init_scale_offset(trace_ymax);
}

// Select data source (simulation or FIFO)
function sel_srce() {
  var sel = elem("sel_srce");
  data_srce = sel.options[sel.selectedIndex].value;
}

// Decode CSV string into floating-point array
function csv_decode(s) {
  data = s.trim().split(',');
  return data.map(x => parseFloat(x));
}

// Convert hex colour to normalised RGB values
function colr(x) {
  return ([(x >> 16 & 255) / 255.0, (x >> 8 & 255) / 255.0, (x & 255) / 255.0, 1.0]);
}

// Return a document element, given ID
function elem(id) {
  return document.getElementById(id);
}

class Oscilloscope {
  gl = null

  xDivisions = 0
  yDivisions = 0
  xGlPerDiv = 0
  yGlPerDiv = 0

  msTime = 0;
  data = [];
  grid_vertices = []
  trace_vertices = []

  constructor() {
    // Re-size canvas
    canvas.width = window.innerWidth - XMARGIN;
    canvas.height = window.innerHeight - YMARGIN;

    this.initCanvas()

    this.gl = canvas.getContext('webgl2')
    init_scale_offset(trace_ymax);
  }

  compileShader(typ, source) {
    var s = gl.createShader(typ);
    gl.shaderSource(s, source);
    gl.compileShader(s);
    if (!gl.getShaderParameter(s, gl.COMPILE_STATUS))
      throw "Could not compile " +
      (typ == gl.VERTEX_SHADER ? "vertex" : "fragment") +
      " shader:\n\n" + gl.getShaderInfoLog(s);
    return (s);
  }

  initCanvas() {
    var vertex_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);

    var frag_shader = this.compileShader(gl.FRAGMENT_SHADER, frag_code);
    var vert_shader = this.compileShader(gl.VERTEX_SHADER, vert_code);
    program = gl.createProgram();
    gl.attachShader(program, vert_shader);
    gl.attachShader(program, frag_shader);
    gl.linkProgram(program);
    gl.useProgram(program);
    gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);

    var coord = gl.getAttribLocation(program, "a_coords");
    gl.vertexAttribPointer(coord, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(coord);

    var clrs = gl.getUniformLocation(program, 'u_colours');
    gl.uniform4fv(clrs, new Float32Array(trace_colours.flat()));

    gl.clearColor(...clear_colour);
  }

  redraw() {
    const xDivisions = Math.ceil(gl.drawingBufferWidth / PX_PER_DIV);
    const yDivisions = Math.floor(gl.drawingBufferHeight / PX_PER_DIV);
    this.xGlPerDiv = (NORM_XMAX - NORM_XMIN) / (gl.drawingBufferWidth / PX_PER_DIV);
    this.yGlPerDiv = (NORM_YMAX - NORM_YMIN) / (gl.drawingBufferHeight / PX_PER_DIV);

    var graph_vertices = [];
    graph_vertices.push(...grid_vertices);
    graph_vertices.push(...trace_vertices);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(graph_vertices), gl.STATIC_DRAW);

    gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);
    gl.clear(gl.COLOR_BUFFER_BIT);
    if (trace_vertices.length)
      gl.drawArrays(gl.LINES, 0, (grid_vertices.length + trace_vertices.length) / 3);
  }
  
  drawGrid() {
    const xDivOffset = (time / MS_PER_DIV) % 1.0;
    for (var i = 0; i <= this.xDivisions; i++) {
      var x = NORM_XMIN + (this.xGlPerDiv * i) - (this.xGlPerDiv * xDivOffset);
      this.drawLine(this.grid_vertices, x, NORM_YMIN, x, 2 * NORM_YMAX, GRID_COLOR);
    }
    for (var i = 0; i <= this.yDivisions; i++) {
      var y = NORM_YMIN + (this.yGlPerDiv * i);
      this.drawLine(this.grid_vertices, NORM_XMIN, y, NORM_XMAX, y, GRID_COLOR);
    }
  }

  drawTrace(data, color) {
    let x, y
  
    for (var n = 0; n < data.length; n++) {
      const [_msTime, value] = data[n];
      const diffTime = this.msTime - _msTime;
  
      if (n > 1)
        set_point(this.trace_vertices, x, y, color);
      x = NORM_XMIN + MS_PER_DIV * diffTime;
      y = value;
      set_point(this.trace_vertices, x, y, color);
    }
  }

  drawPoint(vts, x1, y1, z) {
    vts.push(x1, y1, z);
  }

  drawLine(vts, x1, y1, z, x2, y2, z) {
    vts.push(x1, y1, z, x2, y2, z);
  }
}