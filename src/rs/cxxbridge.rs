#[cxx::bridge(namespace = rustyf4se)]
mod ffi {
    struct SharedThing {
        z: i32,
        y: Box<ThingR>,
        x: UniquePtr<ThingC>,
    }

    unsafe extern "C++" {
        include!("rustyf4se/symbol_export.h");
        include!("rustyf4se/cpp.h");
        include!("rustyf4se.h");

        type ThingC;
        fn make_demo(appname: &str) -> UniquePtr<ThingC>;
        fn get_name(thing: &ThingC) -> &CxxString;
        fn do_thing(state: SharedThing);
    }

    extern "Rust" {
        type ThingR;
        fn print_r(r: &ThingR);

        type ReadOperation;
        fn get_id(self: &ReadOperation) -> u8;
        fn get_num(self: &ReadOperation) -> usize;
        fn new_read_operation(id: u8, num: usize) -> Box<ReadOperation>;

        type WriteOperation;
        fn get_id(self: &WriteOperation) -> u8;
        fn get_num(self: &WriteOperation) -> usize;
        fn new_write_operation(id: u8, num: usize) -> Box<WriteOperation>;
    }

    #[namespace = "rustyf4se::geom"]
    extern "Rust" {
        fn read(buffer: &mut [u32]) -> bool;
        fn write(buffer: &[u32]) -> bool;
    }
}

pub struct ThingR(usize);

pub struct ReadOperation {
    id: u8,
    num: usize,
}

fn read(buffer: &mut [u32]) -> bool {
    for i in 0..buffer.len() {
        println!("read buffer i={}", i);
        buffer[i] = i as u32;
    }
    true
}

fn write(buffer: &[u32]) -> bool {
    for i in 0..buffer.len() {
        println!("write buffer i={} value={}", i, buffer[i]);
    }
    true
}

fn new_read_operation(id: u8, num: usize) -> Box<ReadOperation> {
    println!("new_read_operation(id={0}, num={1})", id, num);
    Box::new(ReadOperation { id, num })
}

impl ReadOperation {
    fn get_id(&self) -> u8 {
        println!("ReadOperation.get_id()");
        self.id
    }

    fn get_num(&self) -> usize {
        println!("ReadOperation.get_num()");
        self.num
    }
}

pub struct WriteOperation {
    id: u8,
    num: usize,
}

fn new_write_operation(id: u8, num: usize) -> Box<WriteOperation> {
    println!("new_write_operation(id={0}, num={1})", id, num);
    Box::new(WriteOperation { id, num })
}

impl WriteOperation {
    fn get_id(&self) -> u8 {
        println!("WriteOperation.get_id()");
        self.id
    }

    fn get_num(&self) -> usize {
        println!("WriteOperation.get_num()");
        self.num
    }
}

fn print_r(r: &ThingR) {
    println!("called back with r={}", r.0);
}

#[allow(dead_code)]
fn my_test() {
    let x = ffi::make_demo("demo of cxx::bridge");
    println!("this is a {}", ffi::get_name(x.as_ref().unwrap()));

    ffi::do_thing(ffi::SharedThing {
        z: 222,
        y: Box::new(ThingR(333)),
        x,
    });
}
