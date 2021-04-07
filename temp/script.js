"use strict";

// simplified on three.js/examples/webgl_loader_gltf2.html                        
function main() {
    // renderer                                                                 
    const renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setSize(800, 600);
    document.body.appendChild(renderer.domElement);

    // camera                                                                   
    const camera = new THREE.PerspectiveCamera(30, 800 / 600, 1, 10000);
    camera.position.set(30, 10, 70); // settings in `sceneList` "Monster"
    camera.up.set(0, 1, 0);
    camera.lookAt(new THREE.Vector3(0, 0, 0));

    // scene and lights                                                         
    const scene = new THREE.Scene();
    scene.add(new THREE.AmbientLight(0xcccccc));

    // load gltf model and texture                            
    const objs = [];
    const loader = new THREE.GLTF2Loader();
    loader.load("./z-Monster.gltf", gltf => {
        // model is a THREE.Group (THREE.Object3D)                              
        const mixer = new THREE.AnimationMixer(gltf.scene);
        // animations is a list of THREE.AnimationClip
        for (const anim of gltf.animations) {
            mixer.clipAction(anim).play();
        }
        // settings in `sceneList` "Monster"
        gltf.scene.scale.set(0.4, 0.4, 0.4);
        gltf.scene.rotation.copy(new THREE.Euler(0, -3 * Math.PI / 4, 0));
        gltf.scene.position.set(2, 1, 0);
        
        scene.add(gltf.scene);
        objs.push({gltf, mixer});
    });

    // animation rendering                                                      
    const clock = new THREE.Clock();
    (function animate() {
        // animation with THREE.AnimationMixer.update(timedelta)                
        objs.forEach(({mixer}) => {mixer.update(clock.getDelta());});
        renderer.render(scene, camera);
        requestAnimationFrame(animate);
    })();
    return objs;
}
const objs = main();