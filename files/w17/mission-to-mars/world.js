//COLORS
var Colors = {
    red:0xf25346,
    white:0xd8d0d1,
    brown:0x59332e,
    pink:0xF5986E,
    brownDark:0x23190f,
    blue:0x68c3c0,
};

// THREEJS RELATED VARIABLES

var scene,
    camera, fieldOfView, aspectRatio, nearRocket, farRocket,
    renderer, container;

//SCREEN & MOUSE VARIABLES

var HEIGHT, WIDTH,
    mousePos = { x: 0, y: 0 };

//INIT THREE JS, SCREEN AND MOUSE EVENTS

function createScene() {

  HEIGHT = window.innerHeight;
  WIDTH = window.innerWidth;

  scene = new THREE.Scene();
  aspectRatio = WIDTH / HEIGHT;
  fieldOfView = 60;
  nearRocket = 1;
  farRocket = 10000;
  camera = new THREE.PerspectiveCamera(
    fieldOfView,
    aspectRatio,
    nearRocket,
    farRocket
    );
  scene.fog = new THREE.Fog(0xf7d9aa, 10, 9500);
  camera.position.x = 0;
  camera.position.z = 200;
  camera.position.y = 100;

  renderer = new THREE.WebGLRenderer({ alpha: true, antialias: true });
  renderer.setClearColor(Colors.brown);
  renderer.setSize(WIDTH, HEIGHT);
  renderer.shadowMap.enabled = true;
  container = document.getElementById('world');
  container.appendChild(renderer.domElement);

  window.addEventListener('resize', handleWindowResize, false);
}

// HANDLE SCREEN EVENTS

function handleWindowResize() {
  HEIGHT = window.innerHeight;
  WIDTH = window.innerWidth;
  renderer.setSize(WIDTH, HEIGHT);
  camera.aspect = WIDTH / HEIGHT;
  camera.updateProjectionMatrix();
}


// LIGHTS

var ambientLight, hemisphereLight, shadowLight;

function createLights() {

  hemisphereLight = new THREE.HemisphereLight(0xaaaaaa,0x000000, .9)
  shadowLight = new THREE.DirectionalLight(0xffffff, .9);
  shadowLight.position.set(150, 350, 350);
  shadowLight.castShadow = true;
  shadowLight.shadow.camera.left = -400;
  shadowLight.shadow.camera.right = 400;
  shadowLight.shadow.camera.top = 400;
  shadowLight.shadow.camera.bottom = -400;
  shadowLight.shadow.camera.near = 1;
  shadowLight.shadow.camera.far = 1000;
  shadowLight.shadow.mapSize.width = 2048;
  shadowLight.shadow.mapSize.height = 2048;

  scene.add(hemisphereLight);
  scene.add(shadowLight);
}

var Rocket = function() {
    this.mesh = new THREE.Object3D();
    this.mesh.name = "airRocket";

    rocketColor = Colors.white;

    // Create the body
    var geomBody = new THREE.BoxGeometry(160, 50, 50, 1, 1, 1);
    var matBody = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var body = new THREE.Mesh(geomBody, matBody);
    body.castShadow = true;
    body.receiveShadow = true;
    this.mesh.add(body);

    // Create nose
    var geomNose = new THREE.ConeGeometry(35.55, 100, 4);
    var matNose = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var nose = new THREE.Mesh(geomNose, matNose);
    nose.position.x = 129;
    nose.castShadow = true;
    nose.receiveShadow = true;
    nose.rotation.x = Math.PI / 4;
    nose.rotation.z = Math.PI * 3 / 2;
    this.mesh.add(nose);

    // Create Fins

    var geomFin1 = new THREE.BoxGeometry(60, 100, 15, 1, 1, 1);
    var matFin1 = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var fin1 = new THREE.Mesh(geomFin1, matFin1);
    fin1.position.set(-80, 0, 0);
    fin1.rotation.x = Math.PI / 4;
    fin1.castShadow = true;
    fin1.receiveShadow = true;
    this.mesh.add(fin1);

    var geomFin2 = new THREE.BoxGeometry(60, 100, 15, 1, 1, 1);
    var matFin2 = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var fin2 = new THREE.Mesh(geomFin2, matFin2);
    fin2.position.set(-80, 0, 0);
    fin2.rotation.x = Math.PI * 3 / 4;
    fin2.castShadow = true;
    fin2.receiveShadow = true;
    this.mesh.add(fin2);
};

Sky = function(){
  this.mesh = new THREE.Object3D();
  this.nClouds = 20;
  this.clouds = [];
  var stepAngle = Math.PI*2 / this.nClouds;
  for(var i=0; i<this.nClouds; i++){
    var c = new Cloud();
    this.clouds.push(c);
    var a = stepAngle*i;
    var h = 750 + Math.random()*200;
    c.mesh.position.y = Math.sin(a)*h;
    c.mesh.position.x = Math.cos(a)*h;
    c.mesh.position.z = -400-Math.random()*400;
    c.mesh.rotation.z = a + Math.PI/2;
    var s = 1+Math.random()*2;
    c.mesh.scale.set(s,s,s);
    this.mesh.add(c.mesh);
  }
}

Sea = function(){
  var geom = new THREE.CylinderGeometry(600,600,800,40,10);
  geom.applyMatrix(new THREE.Matrix4().makeRotationX(-Math.PI/2));
  var mat = new THREE.MeshPhongMaterial({
    color:Colors.red,
    transparent:true,
    opacity:.6,
    shading:THREE.FlatShading,
  });
  this.mesh = new THREE.Mesh(geom, mat);
  this.mesh.receiveShadow = true;
}

Cloud = function(){
  this.mesh = new THREE.Object3D();
  this.mesh.name = "cloud";
  var geom = new THREE.CubeGeometry(20,20,20);
  var mat = new THREE.MeshPhongMaterial({
    color:Colors.pink,
  });

  var nBlocs = 3+Math.floor(Math.random()*3);
  for (var i=0; i<nBlocs; i++ ){
    var m = new THREE.Mesh(geom.clone(), mat);
    m.position.x = i*15;
    m.position.y = Math.random()*10;
    m.position.z = Math.random()*10;
    m.rotation.z = Math.random()*Math.PI*2;
    m.rotation.y = Math.random()*Math.PI*2;
    var s = .1 + Math.random()*.9;
    m.scale.set(s,s,s);
    m.castShadow = true;
    m.receiveShadow = true;
    this.mesh.add(m);
  }
}

// 3D Models
var sea;
var rocket;

function createRocket(){
  rocket = new Rocket();
  rocket.mesh.scale.set(.25,.25,.25);
  rocket.mesh.position.y = 100;
  scene.add(rocket.mesh);
}

function createSea(){
  sea = new Sea();
  sea.mesh.position.y = -600;
  scene.add(sea.mesh);
}

function createSky(){
  sky = new Sky();
  sky.mesh.position.y = -600;
  scene.add(sky.mesh);
}

function loop(){
  updateRocket();
  sea.mesh.rotation.z += .005;
  sky.mesh.rotation.z += .01;
  renderer.render(scene, camera);
  requestAnimationFrame(loop);
}

function updateRocket(){
  var targetY = normalize(mousePos.y,-.75,.75,25, 175);
  var targetX = normalize(mousePos.x,-.75,.75,-100, 100);
  rocket.mesh.position.y = targetY;
  rocket.mesh.position.x = targetX;
  rocket.mesh.rotation.x += 0.002;
}

function normalize(v,vmin,vmax,tmin, tmax){
  var nv = Math.max(Math.min(v,vmax), vmin);
  var dv = vmax-vmin;
  var pc = (nv-vmin)/dv;
  var dt = tmax-tmin;
  var tv = tmin + (pc*dt);
  return tv;
}

function setupWorld(event){
  document.addEventListener('mousemove', handleMouseMove, false);
  createScene();
  createLights();
  createRocket();
  createSea();
  createSky();
  loop();
}

// HANDLE MOUSE EVENTS

var mousePos = { x: 0, y: 0 };

function handleMouseMove(event) {
  var tx = -1 + (event.clientX / WIDTH)*2;
  var ty = 1 - (event.clientY / HEIGHT)*2;
  mousePos = {x:tx, y:ty};
  console.log("in handleMouseMove mousePos: x " + mousePos.x + ", y " + mousePos.y);
}

function handleTouchMove(event) {
    event.preventDefault();
    var tx = -1 + (event.touches[0].pageX / WIDTH)*2;
    var ty = 1 - (event.touches[0].pageY / HEIGHT)*2;
    mousePos = {x:tx, y:ty};
}

//window.addEventListener('load', init, false);