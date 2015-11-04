extern crate rand;

// Traits
use rand::Rng;


const NDIM: usize = 3;
// const NPARTS: usize = 10000;
const NPARTS: usize = 3;
const NSTEPS: i32 = 2;
const MASS: f64 = 1.0;
const DT: f64 = 1e-4;


#[derive(Debug)]
struct Vector {
    x: f64,
    y: f64,
    z: f64,
}

impl Vector {
    fn rand() -> Vector {
        Vector {
            x: rand::thread_rng().gen_range(0.0, 10.0),
            y: rand::thread_rng().gen_range(0.0, 10.0),
            z: rand::thread_rng().gen_range(0.0, 10.0),
        }
    }

    fn zero() -> Vector {
        Vector { x: 0.0, y: 0.0, z: 0.0, }
    }
}


#[derive(Debug)]
struct Particle {
    pos: Vector,
    vel: Vector,
    acc: Vector,
}

impl Particle {
    fn new() -> Particle {
        Particle {
            pos: Vector::rand(),
            vel: Vector::zero(),
            acc: Vector::zero(),
        }
    }
}


#[derive(Debug)]
struct System {
    particles: Vec<Particle>,
    pe: f64,
    ke: f64,
}

impl System {
    fn error(&self, etot: f64) -> f64 {
        (self.pe + self.ke - etot)/etot
    }

    fn step(&mut self) {
        self.pe = 0.0;
        self.ke = 0.0;

        for i in 0..NPARTS {
            for part in self.particles.iter().enumerate().filter_map(|(j, p)| if i != j { Some(p) } else { None }) {
                println!("{} {:?}", i, part);
            }
        }
    }
}


fn main() {
    let mut particles = Vec::with_capacity(NPARTS);
    for _ in 0..NPARTS {
        particles.push(Particle::new());
    }
    let mut system = System { particles: particles, ke: 0.0, pe: 0.0, };
    system.step();
    let etot = system.pe + system.ke;

    println!("PE\tKE\tError");
    for _ in 0..NSTEPS {
        system.step();
        println!("{}\t{}\t{}", system.pe, system.ke, system.error(etot));
    }
}
