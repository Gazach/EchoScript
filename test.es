## ini komen
println("Teks ini berasal dari EchoScript!");

let a = 1000; ## tes variable tipe data int
let b = 200;

func tambah(a, b){ ## tes penjumlahan
    return a + b;
}
func kurang(a, b){ ## tes pengurangan
    return a - b;
}
func bagi(a, b){ ## tes pembagian
    return a / b;
}
func kali(a, b){ ## tes perkalian
    return a * b;
}
## output dari function
println(tambah(a, b)); 
println(kurang(a, b));
println(bagi(a, b));
println(kali(a, b));

let printdaristring = "teks ini langsung dari string!"; ## tes variable tipe data string

println(printdaristring); ## print dari string

let databool = false; ## tes variable tipe data boolean
println("databool adalah : "+databool);