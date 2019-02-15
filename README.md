# mod_CM
mix(o0..o5) submodel for paq

[code]
         book1             wcc386
paq8p_-- 192227  28.734s   194069  23.469s
paq8p_-C 191819  33.656s   193960  27.344s
paq8p_P- 191293  33.968s   193781  27.235s
paq8p_PC 191151  38.641s   193763  30.828s
[/code]

"P" = mod_ppmd present
"C" = mod_CM present

The model uses precise 8-byte counters and a binary mixer tree,
and a relatively new trick of "probability extrapolation", which
is also applied to mixer weights (binary mixer weights are actually
probabilities of one of inputs being better for coding).
