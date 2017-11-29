#!/usr/bin/perl -w

use strict 'vars';

# OUT
my $tx_path  = "/sys/class/net/eth0/statistics/tx_bytes";
print "Running benchmark on: $tx_path ...\n";

my $bytes = 0;
for (my $i = 0; $i < 5; $i++) {
  open my $fh, '<', $tx_path or die "Failed to open file '$tx_path'! $!\n";
  $bytes += <$fh>;
  close $fh;
  sleep 2;
}

my $tx_speed = sprintf("%0.2f MB/s", $bytes / 5 / 1024.0 / 1024.0);
printf "measured speed: %s\n", $tx_speed;

# IN 
my $rx_path  = "/sys/class/net/eth0/statistics/rx_bytes";
print "Running benchmark on: $rx_path ...\n";

$bytes = 0;
for (my $i = 0; $i < 5; $i++) {
  open my $fh, '<', $rx_path or die "Failed to open file '$rx_path'! $!\n";
  $bytes += <$fh>;
  close $fh;
  sleep 2;
}

my $rx_speed = sprintf("%0.2f MB/s", $bytes / 5 / 1024.0 / 1024.0);
printf "measured speed: %s\n", $rx_speed;


system("echo mosquitto_pub -h hautomation01.hacklabor.de -t 'network/torrelay01/traffic' -m \"\\/ $rx_speed  /\\ $tx_speed\"")
