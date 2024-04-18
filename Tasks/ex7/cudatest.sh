
#!/bin/bash
START=1
END=10
PSTART=1
PEND=7

rm -rfv Results
mkdir Results
rm -rfv Outputs
mkdir Outputs

for(( p=$PSTART; p<=$PEND; p++ ))
do
    echo "Threads\tSeconds" > "Outputs/${p}_output.txt"
    for (( i=$START; i<=$END; i++ ))
    do
        for (( j=$START; j<=$END; j++ ))
            do
                for (( k=$START; k<=$END; k++ ))
                    do
                        for (( l=$START; l<=$END; l++ ))
                            do
                                result=$(( i*j*k*l))
                                
                                if [ $(cat "Outputs/${p}_output.txt"| grep -cix "$result") -le 0 ]; then
                                    echo "$p : $i * $j * $k * $l = $result";
                                    echo "$result">>"Outputs/${p}_output.txt"
                                    ./main_md5 $p $i $j $k $l>>"Results/${p}_longs.csv"
                                else
								echo "$p : $i * $j * $k * $l = $result NOT USED";
                                fi
                            done
                    done
            done
    done
done 
