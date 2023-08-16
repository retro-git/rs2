# Use Ubuntu as the base image
FROM ubuntu:latest

# # Set environment variables
# ENV PATH="/opt/mips-gcc/bin:${PATH}"

# # Update and install required packages
# RUN apt-get update && \
#     apt-get install -y python3 python3-pip git wget build-essential xdelta3 && \
#     apt-get clean

# # Clone the psx-modding-toolchain repository
# RUN git clone https://github.com/mateusfavarin/psx-modding-toolchain /psx-modding-toolchain

# # Install required Python packages
# RUN pip3 install requests opencv-python pymkpsxiso pyxdelta pillow

# # Get and run the spawn-compiler.sh script
# RUN wget https://github.com/grumpycoders/pcsx-redux/raw/main/tools/linux-mips/spawn-compiler.sh && \
#     chmod +x spawn-compiler.sh && \
#     ./spawn-compiler.sh && \
#     rm spawn-compiler.sh

# # Clone rs2 inside the "spyro2" folder
# RUN mkdir -p /psx-modding-toolchain/games/spyro2 && \
#     cd /psx-modding-toolchain/games/spyro2 && \
#     git clone https://github.com/retro-git/rs2