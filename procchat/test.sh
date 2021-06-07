./procchat &
sleep 1
echo "test connection starts"
./clients/client1
sleep 1
./clients/client2 
sleep 1
pid=$(pgrep -f procchat)
kill $pid
ls dir1 > file1
ls dir2 >> file1
rm -r dir1 
rm -r dir2 
diff file1 correct/file_1
echo "test connnection passed"
echo "test Ping Pong 1 starts"
./procchat &
sleep 1
./clients/client1
sleep 16
ls -l dir1 |grep "^-" | wc -l > file2
diff file2 correct/file_2
echo "test Ping Pong 1 passed"
pid1=$(pgrep -f procchat)
echo "test Ping Pong 2 starts"
kill $pid1
./procchat &
sleep 1
./clients/client3
sleep 2
ls dir3 > file3
diff file3 correct/file_3
sleep 20
ls -l dir1 |grep "^-" | wc -l > file4
diff file4 correct/file_4
echo "test Ping Pong 2 passed"
pid2=$(pgrep -f procchat)
kill $pid2
echo "test Message starts"
./procchat &
sleep 1
./clients/client4
sleep 1
./clients/client5
echo "test Message passed"
pid3=$(pgrep -f procchat)
kill $pid3

rm -r dir1 dir3

rm file1 file2 file3 file4





