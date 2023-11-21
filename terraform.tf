provider "aws" {
  region = "us-east-2"
}

resource "aws_instance" "example" {
  count         = 1
  ami           = "ami-0b817788973ebfc47"
  instance_type = "t2.micro"
  vpc_security_group_ids = ["sg-0e7a9a7eca15be067"]
  key_name      = "cluster-opencv-key"

  tags = {
    Name = "Node ${count.index}"
  }
}
