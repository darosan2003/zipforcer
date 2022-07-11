use std::env;
use std::fs::File;
use std::io::BufReader;
use std::process::exit;
use std::io::prelude::*;
use std::process::Command;

fn bruteforce(zipfile: &String, passlist: &String) {
    let mut attempt = 1;

    let file = File::open(passlist)
        .expect("Failed to open file");

    let reader = BufReader::new(file);

    for pass in reader.lines() {
        let pass = pass.unwrap();
        let password = pass.as_str();

        println!("[{}] Trying {}...", attempt, password); 

        let output = Command::new("unzip")
            .args(["-P", password, zipfile])
            .output()
            .expect("Failed to execute command");

        if output.status.success() == true {
            println!("\n[+] Password found: {}", password);
            exit(0);
        }

        attempt += 1;

    }
    
}

fn main() {
    let argv: Vec<String> = env::args().collect();

    if argv.len() != 3 {
        println!("Error. Incorrect number of arguments. Expected 3");
        println!("Usage: {} <zipfile> <passlist>", argv[0]);
        exit(1);
    }

    bruteforce(&argv[1], &argv[2]);
}
