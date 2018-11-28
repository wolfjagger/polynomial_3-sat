#############################################################################
# Disclaimer:THE SAMPLE CODE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED #
# WARRANTIES,  INCLUDING  THE  IMPLIED  WARRANTIES OF  MERCHANTABILITY  AND #
# FITNESS FOR A  PARTICULAR PURPOSE  ARE DISCLAIMED. IN  NO EVENT SHALL THE #
# AUTHORS  BE  LIABLE  FOR  ANY  DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,#
# EXEMPLARY,  OR  CONSEQUENTIAL   DAMAGES  (INCLUDING,  BUT  NOT LIMITED TO,#
# PROCUREMENT  OF  SUBSTITUTE   GOODS  OR SERVICES;  LOSS  OF USE, DATA, OR #
# PROFITS;  OR BUSINESS  INTERRUPTION)  SUSTAINED  BY YOU OR A  THIRD PARTY,#
# HOWEVER CAUSED AND ON ANY  THEORY OF  LIABILITY,  WHETHER   IN   CONTRACT,#
# STRICT LIABILITY,OR TORT ARISING IN ANY WAY OUT OF THE USE OF THIS SAMPLE #
# CODE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                  #
#############################################################################

cmake_minimum_required(VERSION 3.9)
project(solver)

include_directories(/usr/local/include/)
include_directories(/usr/include/)
set(CMAKE_CXX_STANDARD 17)

add_executable(solver main.cpp DimacsReader.cpp DimacsReader.h Utilities.cpp Utilities.h Preprocessor.cpp Preprocessor.h EdgeSequesnce.cpp EdgeSequesnce.h clause.h LiteralsMap.h EdgeSequenceSack.cpp EdgeSequenceSack.h VertexSequence.cpp VertexSequence.h)
