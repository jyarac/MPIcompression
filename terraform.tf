provider "aws" {
  region = "us-east-2"
}

data "aws_efs_file_system" "efs" {
  file_system_id = "fs-0df02f89d3280c79d" 
}

resource "aws_instance" "nodes" {
  count         = 2
  ami           = "ami-0b817788973ebfc47"
  instance_type = "t2.micro"
  vpc_security_group_ids = ["sg-0e7a9a7eca15be067"]
  key_name      = "cluster-opencv-key"

  tags = {
    Name = "Node ${count.index}"
  }

  user_data = <<-EOF
              #!/bin/bash
              cd /home/ubuntu/build
              sudo make isntall
              export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
              yum install -y amazon-efs-utils
              mkdir /mnt/efs
              mount -t efs ${data.aws_efs_file_system.efs.dns_name}:/ /mnt/efs
              EOF
}

