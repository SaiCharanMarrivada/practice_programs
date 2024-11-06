using BenchmarkTools

n = 1000000
@btime @ccall "./int_sqrt.so".int_sqrt($n::UInt32)::UInt32
@btime floor(sqrt(n))
