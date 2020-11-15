### Host → Guest

#### How to compile

<pre>
sudo gcc transfer_bulk_data.c -o transfer_bulk_data -lusb
</pre>

#### How to run

<pre>
sudo ./transfer_bulk_data
</pre>

### Guest → Host

#### How to compile

<pre>
sudo gcc receive_bulk_data.c -o receive_bulk_data -lusb
</pre>

#### How to run

<pre>
sudo ./receive_bulk_data
</pre>
