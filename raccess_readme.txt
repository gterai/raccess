Raccess


Raccess computes the accessibility of segment [a, b] = [x, x+l-1] in the transcript
for all the positions x with fixed length l .
prob([a, b]) = sum_{s in S[a, b]} exp(-E(s)/RT) / sum_{s in S0} exp(-E(s)/RT)
where S0 is all the possible secondary structures of the transcript
and S[a, b] is all the secondary strutures having range [a, b] as loop
region.

Thermodynamic energy that is required to keep range [a, b] being accessible
is given by 
access_energy([a, b]) = -RT log(prob([a, b]))

For convenience, Raccess provides an option (-bind_range)
to computes the binding energy with a complementary DNA fragment
in the accessible region.

                 <-      -> bind range
                 __________
            a    ||||||||||       b
------------|---------------------|----------
(( )) () ()                         ()   ()



Usage:
run_raccess [options]

 option:
  -seqfile=<fasta file>      : input sequences in fasta format.
  -access_len=<integer>      : contiguous length over which 
                               the transcript is accesible [default: 50]
  -outfile=<directory>       : output directory [default: ./]
  -bind_range=<first>:<last> : for each segment of accessibility computation, 
                               the binding energy between the region [first, last]
                               (in 1-based, inclusive-end, coordinates
                                relative to the segment)
                               with a complementary DNA/RNA fragment
                               is calculated [default: none]
  -bind_dna=<bool>           : if true, binding energy is computed for a
                               complementary DNA fragment 
                               if false, binding energy is computed
                               for a complementary RNA fragment. [default: true]

  -max_span=<integer>        : maximal span of base pairs considered [default: 100]
  -energy_thr=<double>       : only output the results below the
                               specified energy threshold (unit: kcal/mol) [default: 100]

Output:
        file: outdir/raccess_out.txt
>fasta_header
pos1	len1,ene11;len2,ene12;
pos2	len1,ene21;len2,ene22;


	field description:
	  pos          :  sequence position x in 0-based coordinate
                          accessibility of the range [x, x + access_len - 1] is computed
          len: access_len
          ene:  free energy to access in this region calculated from the formula: 
                          energy [kcal/mol] = - RT log(probability) with RT=0.61633008[kcal/mol]

        There is a new line at the end of each fasta sequence.

Example:
./src/raccess/run_raccess -outfile=./example/raccess_out.txt -seqfile=./example/dna.fa -access_len=5,10,20 -max_span=80 
		   
raccess_out.txt
----------------------------------------
>AB000263
350     5,0.587571;
349     5,0.671838;
348     5,0.688657;
347     5,0.632836;
346     5,0.632803;
345     5,0.742851;10,0.856473;
344     5,0.761075;10,0.867324;
343     5,1.05019;10,1.15432;
342     5,0.906549;10,1.16339;
341     5,0.674628;10,1.10179;
340     5,0.563941;10,1.21421;
339     5,0.7748;10,1.43912;
338     5,0.855941;10,1.46161;
337     5,1.50655;10,1.81358;
336     5,1.77874;10,2.04356;
335     5,1.78954;10,1.86431;20,2.0987;
334     5,2.6894;10,2.77606;20,3.01488;
333     5,3.38362;10,3.46585;20,3.71417;
332     5,3.39939;10,3.49725;20,3.74974;
331     5,2.55136;10,3.46097;20,3.74418;
330     5,2.7096;10,3.67404;20,3.98726;
329     5,2.84122;10,3.8007;20,4.13477;
