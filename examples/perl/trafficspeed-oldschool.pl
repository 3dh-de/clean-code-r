#!/usr/bin/perl -w

# EDIT HERE:
my $device  = "eth0";
my $seconds = 10;

($seconds > 4) or die "Benchmark has to run min. 4 seconds!";


# function to run simple benchmark
sub benchmark() {
  my $path  = shift(@_) or die "Missing path argument for benchmark()!";
  my $runs  = $seconds / 2;
  my $bytes = 0;

  print "Running benchmark on: $path ...\n";
  for (my $i = 0; $i < $runs; $i++) {
    open my $fh, '<', $path or die "Failed to open file '$path'! $!\n";
    $bytes += <$fh>;
    close $fh;
    sleep 2;
  }

  my $speed = sprintf("%0.2f MB/s", $bytes < $runs ? 0 : $bytes / $runs / 1024.0 / 1024.0);

  printf "measured speed: %s\n", $speed;

  return $speed;
}


my $tx_speed = &benchmark("/sys/class/net/${device}/statistics/tx_bytes");
my $rx_speed = &benchmark("/sys/class/net/${device}/statistics/rx_bytes");

system("echo mosquitto_pub -h hautomation01.hacklabor.de -t 'network/torrelay01/traffic' -m \"\\/ $rx_speed  /\\ $tx_speed\"")
