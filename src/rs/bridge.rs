use ffi::{log_info, log_warning};

#[cxx::bridge(namespace = bridge)]
mod ffi {
    unsafe extern "C++" {
        include!("bridge.h");

        fn log_info(text: &str);
        fn log_warning(text: &str);
    }

    extern "Rust" {
        fn introduce_yourself();
    }
}

fn introduce_yourself() {
    log_info("Hello! This is Rust speaking!");
    log_warning("We are most definitely not replacing C++!");
}
