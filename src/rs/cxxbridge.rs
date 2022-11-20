use ffi::{log_info, log_warning};

#[cxx::bridge(namespace = rustyf4se)]
mod ffi {
    unsafe extern "C++" {
        include!("rustyf4se/symbol_export.h");
        include!("rustyf4se/cpp.h");
        include!("rustyf4se.h");

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
