Vagrant.configure("2") do |config|
    config.vm.box = "bento/ubuntu-22.04-arm64"  # You can choose a different Ubuntu version if you prefer
  
    config.vm.network "private_network", type: "dhcp"
  
    config.vm.provider "parallels" do |vb|
      vb.memory = "8092"
      vb.cpus = 4
    end
  
    config.vm.provision "ansible" do |ansible|
      ansible.playbook = "provision.yml"
    end
  end