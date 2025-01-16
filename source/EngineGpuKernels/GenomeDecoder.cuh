#pragma once

#include <nppdefs.h>

#include "EngineInterface/CellTypeConstants.h"
#include "EngineInterface/GenomeConstants.h"
#include "Base.cuh"
#include "Object.cuh"

class GenomeDecoder
{
public:
    //genome-wide methods
    template <typename Func>
    __inline__ __device__ static void executeForEachNode(uint8_t* genome, int genomeSize, Func func);
    template <typename Func>
    __inline__ __device__ static void executeForEachNodeRecursively(uint8_t* genome, int genomeSize, bool includedSeparatedParts, bool countBranches, Func func);
    __inline__ __device__ static GenomeHeader readGenomeHeader(ConstructorType const& constructor);
    __inline__ __device__ static int getGenomeDepth(uint8_t* genome, int genomeSize);
    __inline__ __device__ static int getNumNodesRecursively(uint8_t* genome, int genomeSize, bool includeRepetitions, bool includedSeparatedParts);
    __inline__ __device__ static int getRandomGenomeNodeAddress(
        SimulationData& data,
        uint8_t* genome,
        int genomeSize,
        bool considerZeroSubGenomes,
        int* subGenomesSizeIndices = nullptr,
        int* numSubGenomesSizeIndices = nullptr,
        int randomRefIndex = 0);
    __inline__ __device__ static int getNumNodes(uint8_t* genome, int genomeSize);
    __inline__ __device__ static int getNodeAddress(uint8_t* genome, int genomeSize, int nodeIndex);
    __inline__ __device__ static bool isFirstNode(ConstructorType const& constructor);
    __inline__ __device__ static bool isFirstRepetition(ConstructorType const& constructor);
    __inline__ __device__ static bool isLastNode(ConstructorType const& constructor);
    __inline__ __device__ static bool isLastRepetition(ConstructorType const& constructor);
    __inline__ __device__ static bool hasInfiniteRepetitions(ConstructorType const& constructor);
    __inline__ __device__
        static bool hasEmptyGenome(ConstructorType const& constructor);
    __inline__ __device__ static bool isFinished(ConstructorType const& constructor);
    template <typename ConstructorOrInjector>
    __inline__ __device__ static bool containsSelfReplication(ConstructorOrInjector const& cellType);
    template <typename CellTypeSource, typename CellTypeTarget>
    __inline__ __device__ static void copyGenome(SimulationData& data, CellTypeSource& source, int genomeBytePosition, CellTypeTarget& target);
    __inline__ __device__ static bool isSeparating(uint8_t* genome);
    __inline__ __device__ static int getNumRepetitions(uint8_t* genome, bool countInfinityAsOne = false);
    __inline__ __device__ static int getNumBranches(uint8_t* genome);

    //node-wide methods
    __inline__ __device__ static int getNextCellTypeDataSize(uint8_t* genome, int genomeSize, int nodeAddress, bool withSubgenome = true);
    __inline__ __device__ static CellType getNextCellType(uint8_t* genome, int nodeAddress);
    __inline__ __device__ static bool isNextCellSelfReplication(uint8_t* genome, int nodeAddress);
    __inline__ __device__ static int getNextCellColor(uint8_t* genome, int nodeAddress);
    __inline__ __device__ static int getNextExecutionNumber(uint8_t* genome, int nodeAddress);
    __inline__ __device__ static int getNextInputExecutionNumber(uint8_t* genome, int nodeAddress);
    __inline__ __device__ static void setNextCellType(uint8_t* genome, int nodeAddress, CellType cellType);
    __inline__ __device__ static void setNextCellSelfReplication(uint8_t* genome, int nodeAddress, bool value);
    __inline__ __device__ static void setNextCellSubgenomeSize(uint8_t* genome, int nodeAddress, int size);
    __inline__ __device__ static void setNextCellColor(uint8_t* genome, int nodeAddress, int color);
    __inline__ __device__ static void setNextInputExecutionNumber(uint8_t* genome, int nodeAddress, int value);
    __inline__ __device__ static void setNextOutputBlocked(uint8_t* genome, int nodeAddress, bool value);
    __inline__ __device__ static void setNextAngle(uint8_t* genome, int nodeAddress, uint8_t angle);
    __inline__ __device__ static void setNextRequiredConnections(uint8_t* genome, int nodeAddress, uint8_t angle);
    __inline__ __device__ static void setNextConstructionAngle1(uint8_t* genome, int nodeAddress, uint8_t angle);
    __inline__ __device__ static void setNextConstructionAngle2(uint8_t* genome, int nodeAddress, uint8_t angle);
    __inline__ __device__ static void setNextConstructorSeparation(uint8_t* genome, int nodeAddress, bool separation);
    __inline__ __device__ static void setNextConstructorNumBranches(uint8_t* genome, int nodeAddress, int numBranches);
    __inline__ __device__ static void setNextConstructorNumRepetitions(uint8_t* genome, int nodeAddress, int numRepetitions);
    __inline__ __device__ static bool containsSectionSelfReplication(uint8_t* genome, int genomeSize);
    __inline__ __device__ static int getNodeAddressForSelfReplication(uint8_t* genome, int genomeSize, bool& containsSelfReplicator);
    __inline__ __device__ static void setRandomCellTypeData(SimulationData& data, uint8_t* genome, int nodeAddress, CellType const& cellType, bool makeSelfCopy, int subGenomeSize);
    __inline__ __device__ static int getCellTypeDataSize(
        CellType cellType,
        bool makeSelfCopy,
        int genomeSize);  //genomeSize only relevant for cellType = constructor or injector
    __inline__ __device__ static int
    getNextSubGenomeSize(uint8_t* genome, int genomeSize, int nodeAddress);  //prerequisites: (constructor or injector) and !makeSelfCopy

    //low level read-write methods
    __inline__ __device__ static bool readBool(ConstructorType& constructor, int& genomeBytePosition);
    __inline__ __device__ static uint8_t readByte(ConstructorType& constructor, int& genomeBytePosition);
    __inline__ __device__ static int readOptionalByte(ConstructorType& constructor, int& genomeBytePosition);
    __inline__ __device__ static int readOptionalByte(ConstructorType& constructor, int& genomeBytePosition, int moduloValue);
    __inline__ __device__ static int readWord(ConstructorType& constructor, int& genomeBytePosition);
    __inline__ __device__ static float readFloat(ConstructorType& constructor, int& genomeBytePosition);  //return values from -1 to 1
    __inline__ __device__ static float readEnergy(ConstructorType& constructor, int& genomeBytePosition);  //return values from 36 to 1060
    __inline__ __device__ static float readAngle(ConstructorType& constructor, int& genomeBytePosition);
    __inline__ __device__ static int readWord(uint8_t* genome, int nodeAddress);
    __inline__ __device__ static void writeWord(uint8_t* genome, int address, int word);

    //conversion methods
    __inline__ __device__ static float convertByteToFloat(uint8_t b);
    __inline__ __device__ static uint8_t convertFloatToByte(float b);
    __inline__ __device__ static bool convertByteToBool(uint8_t b);
    __inline__ __device__ static uint8_t convertBoolToByte(bool value);
    __inline__ __device__ static int convertBytesToWord(uint8_t b1, uint8_t b2);
    __inline__ __device__ static void convertWordToBytes(int word, uint8_t& b1, uint8_t& b2);
    __inline__ __device__ static uint8_t convertAngleToByte(float angle);
    __inline__ __device__ static float convertByteToAngle(uint8_t b);
    __inline__ __device__ static uint8_t convertOptionalByteToByte(int value);

    static auto constexpr MAX_SUBGENOME_RECURSION_DEPTH = 15;

private:
    __inline__ __device__ static int findStartNodeAddress(uint8_t* genome, int genomeSize, int refIndex);
};

/************************************************************************/
/* Implementation                                                       */
/************************************************************************/
template <typename Func>
__inline__ __device__ void GenomeDecoder::executeForEachNode(uint8_t* genome, int genomeSize, Func func)
{
    for (int currentNodeAddress = Const::GenomeHeaderSize; currentNodeAddress < genomeSize;) {
        currentNodeAddress += Const::CellBasicBytes + GenomeDecoder::getNextCellTypeDataSize(genome, genomeSize, currentNodeAddress);

        func(currentNodeAddress);
    }
}

template <typename Func>
__inline__ __device__ void GenomeDecoder::executeForEachNodeRecursively(uint8_t* genome, int genomeSize, bool includedSeparatedParts, bool countBranches, Func func)
{
    CUDA_CHECK(genomeSize >= Const::GenomeHeaderSize)

    int subGenomeEndAddresses[MAX_SUBGENOME_RECURSION_DEPTH];
    int subGenomeNumRepetitions[MAX_SUBGENOME_RECURSION_DEPTH + 1];
    int depth = 0;
    subGenomeNumRepetitions[0] = GenomeDecoder::getNumRepetitions(genome, true);
    for (auto nodeAddress = Const::GenomeHeaderSize; nodeAddress < genomeSize;) {
        auto cellType = GenomeDecoder::getNextCellType(genome, nodeAddress);
        func(depth, nodeAddress, subGenomeNumRepetitions[depth]);

        bool goToNextSibling = true;
        if (cellType == CellType_Constructor || cellType == CellType_Injector) {
            auto cellTypeFixedBytes = cellType == CellType_Constructor ? Const::ConstructorFixedBytes : Const::InjectorFixedBytes;
            auto makeSelfCopy = GenomeDecoder::convertByteToBool(genome[nodeAddress + Const::CellBasicBytes + cellTypeFixedBytes]);
            if (!makeSelfCopy) {
                auto deltaSubGenomeStartPos = Const::CellBasicBytes + cellTypeFixedBytes + 3;
                if (!includedSeparatedParts && GenomeDecoder::isSeparating(genome + nodeAddress + deltaSubGenomeStartPos)) {
                    //skip scanning sub-genome
                } else {
                    auto subGenomeSize = GenomeDecoder::getNextSubGenomeSize(genome, genomeSize, nodeAddress);
                    nodeAddress += deltaSubGenomeStartPos;
                    subGenomeEndAddresses[depth++] = nodeAddress + subGenomeSize;

                    auto numBrachnes = countBranches ? GenomeDecoder::getNumBranches(genome + nodeAddress) : 1;
                    auto numRepetitions = GenomeDecoder::getNumRepetitions(genome + nodeAddress, true);
                    subGenomeNumRepetitions[depth] = subGenomeNumRepetitions[depth - 1] * numRepetitions * numBrachnes;
                    nodeAddress += Const::GenomeHeaderSize;
                    goToNextSibling = false;
                }
            }
        }
        if (goToNextSibling) {
            nodeAddress += Const::CellBasicBytes + GenomeDecoder::getNextCellTypeDataSize(genome, genomeSize, nodeAddress);
        }
        for (int i = 0; i < MAX_SUBGENOME_RECURSION_DEPTH && depth > 0; ++i) {
            if (depth > 0) {
                if (subGenomeEndAddresses[depth - 1] == nodeAddress) {
                    --depth;
                } else {
                    break;
                }
            }
        }
    }
}

__inline__ __device__ int GenomeDecoder::getGenomeDepth(uint8_t* genome, int genomeSize)
{
    auto result = 0;
    executeForEachNodeRecursively(genome, genomeSize, true, false, [&result](int depth, int nodeAddress, int repetition) { result = max(result, depth); });
    return result;
}

__inline__ __device__ int GenomeDecoder::getNumNodesRecursively(uint8_t* genome, int genomeSize, bool includeRepetitions, bool includedSeparatedParts)
{
    auto result = 0;
    if (!includeRepetitions) {
        executeForEachNodeRecursively(
            genome, genomeSize, includedSeparatedParts, true, [&result](int depth, int nodeAddress, int repetitions) { ++result; });
    } else {
        executeForEachNodeRecursively(
            genome, genomeSize, includedSeparatedParts, true, [&result](int depth, int nodeAddress, int repetitions) { result += repetitions; });
    }
    return result;
}

__inline__ __device__ int GenomeDecoder::getRandomGenomeNodeAddress(
    SimulationData& data,
    uint8_t* genome,
    int genomeSize,
    bool considerZeroSubGenomes,
    int* subGenomesSizeIndices,
    int* numSubGenomesSizeIndices,
    int randomRefIndex)
{
    if (numSubGenomesSizeIndices) {
        *numSubGenomesSizeIndices = 0;
    }
    CUDA_CHECK(genomeSize >= Const::GenomeHeaderSize)

    if (genomeSize == Const::GenomeHeaderSize) {
        return Const::GenomeHeaderSize;
    }
    if (randomRefIndex == 0) {
        randomRefIndex = data.numberGen1.random(genomeSize - 1);
    }

    int result = 0;
    for (int depth = 0; depth < MAX_SUBGENOME_RECURSION_DEPTH; ++depth) {
        auto nodeAddress = findStartNodeAddress(genome, genomeSize, randomRefIndex);
        result += nodeAddress;
        auto cellType = getNextCellType(genome, nodeAddress);

        if (cellType == CellType_Constructor || cellType == CellType_Injector) {
            auto cellTypeFixedBytes = cellType == CellType_Constructor ? Const::ConstructorFixedBytes : Const::InjectorFixedBytes;
            auto makeSelfCopy = GenomeDecoder::convertByteToBool(genome[nodeAddress + Const::CellBasicBytes + cellTypeFixedBytes]);
            if (makeSelfCopy) {
                break;
            } else {
                if (nodeAddress + Const::CellBasicBytes + cellTypeFixedBytes > randomRefIndex) {
                    break;
                }
                if (numSubGenomesSizeIndices) {
                    subGenomesSizeIndices[*numSubGenomesSizeIndices] = result + Const::CellBasicBytes + cellTypeFixedBytes + 1;
                    ++(*numSubGenomesSizeIndices);
                }
                auto subGenomeStartIndex = nodeAddress + Const::CellBasicBytes + cellTypeFixedBytes + 3;
                auto subGenomeSize = getNextSubGenomeSize(genome, genomeSize, nodeAddress);
                if (subGenomeSize == Const::GenomeHeaderSize) {
                    if (considerZeroSubGenomes && data.numberGen1.randomBool()) {
                        result += Const::CellBasicBytes + cellTypeFixedBytes + 3 + Const::GenomeHeaderSize;
                    } else {
                        if (numSubGenomesSizeIndices) {
                            --(*numSubGenomesSizeIndices);
                        }
                    }
                    break;
                }
                genomeSize = subGenomeSize;
                genome = genome + subGenomeStartIndex;
                randomRefIndex -= subGenomeStartIndex;
                result += Const::CellBasicBytes + cellTypeFixedBytes + 3;
            }
        } else {
            break;
        }
    }
    return result;
}

__inline__ __device__ bool GenomeDecoder::readBool(ConstructorType& constructor, int& genomeBytePosition)
{
    return convertByteToBool(readByte(constructor, genomeBytePosition));
}

__inline__ __device__ uint8_t GenomeDecoder::readByte(ConstructorType& constructor, int& genomeBytePosition)
{
    if (isFinished(constructor)) {
        return 0;
    }
    uint8_t result = constructor.genome[genomeBytePosition++];
    return result;
}

__inline__ __device__ int GenomeDecoder::readOptionalByte(ConstructorType& constructor, int& genomeBytePosition)
{
    auto result = static_cast<int>(readByte(constructor, genomeBytePosition));
    result = result > 127 ? -1 : result;
    return result;
}

__inline__ __device__ int GenomeDecoder::readOptionalByte(ConstructorType& constructor, int& genomeBytePosition, int moduloValue)
{
    auto result = static_cast<int>(readByte(constructor, genomeBytePosition));
    result = result > 127 ? -1 : result % moduloValue;
    return result;
}

__inline__ __device__ int GenomeDecoder::readWord(ConstructorType& constructor, int& genomeBytePosition)
{
    auto b1 = readByte(constructor, genomeBytePosition);
    auto b2 = readByte(constructor, genomeBytePosition);
    return convertBytesToWord(b1, b2);
}

__inline__ __device__ float GenomeDecoder::readFloat(ConstructorType& constructor, int& genomeBytePosition)
{
    return convertByteToFloat(readByte(constructor, genomeBytePosition));
}

__inline__ __device__ float GenomeDecoder::readEnergy(ConstructorType& constructor, int& genomeBytePosition)
{
    return readFloat(constructor, genomeBytePosition) * 100 + 150.0f;
}

__inline__ __device__ float GenomeDecoder::readAngle(ConstructorType& constructor, int& genomeBytePosition)
{
    return convertByteToAngle(readByte(constructor, genomeBytePosition));
}

__inline__ __device__ bool GenomeDecoder::isFirstNode(ConstructorType const& constructor)
{
    return constructor.genomeCurrentNodeIndex == 0;
}

__inline__ __device__ bool GenomeDecoder::isFirstRepetition(ConstructorType const& constructor)
{
    return constructor.genomeCurrentRepetition == 0;
}

__inline__ __device__ bool GenomeDecoder::isLastNode(ConstructorType const& constructor)
{
    if (hasEmptyGenome(constructor)) {
        return true;
    }
    auto nodeAddress = GenomeDecoder::getNodeAddress(constructor.genome, constructor.genomeSize, constructor.genomeCurrentNodeIndex);
    auto nextNodeBytes = Const::CellBasicBytes + getNextCellTypeDataSize(constructor.genome, constructor.genomeSize, nodeAddress);
    return nodeAddress + nextNodeBytes >= constructor.genomeSize;
}

__inline__ __device__ bool GenomeDecoder::isLastRepetition(ConstructorType const& constructor)
{
    return getNumRepetitions(constructor.genome) - 1 == constructor.genomeCurrentRepetition;
}

__inline__ __device__ bool GenomeDecoder::hasInfiniteRepetitions(ConstructorType const& constructor)
{
    return getNumRepetitions(constructor.genome) == NPP_MAX_32S;
}

__inline__ __device__ bool GenomeDecoder::hasEmptyGenome(ConstructorType const& constructor)
{
    if (constructor.genomeSize <= Const::GenomeHeaderSize) {
        CUDA_CHECK(constructor.genomeSize == Const::GenomeHeaderSize)
        CUDA_CHECK(constructor.genomeCurrentNodeIndex == 0)
        return true;
    }
    return false;
}

__inline__ __device__ bool GenomeDecoder::isFinished(ConstructorType const& constructor)
{
    if (hasEmptyGenome(constructor)) {
        return true;
    }
    return getNumBranches(constructor.genome) <= constructor.currentBranch;
}

template <typename CellTypeSource, typename CellTypeTarget>
__inline__ __device__ void GenomeDecoder::copyGenome(SimulationData& data, CellTypeSource& source, int genomeBytePosition, CellTypeTarget& target)
{
    bool makeGenomeCopy = readBool(source, genomeBytePosition);
    if (!makeGenomeCopy) {
        auto size = readWord(source, genomeBytePosition);
        target.genomeSize = size;
        target.genome = data.objects.auxiliaryData.getAlignedSubArray(size);
        //#TODO can be optimized
        for (int i = 0; i < size; ++i) {
            target.genome[i] = source.genome[genomeBytePosition + i];
        }
    } else {
        auto size = source.genomeSize;
        target.genomeSize = size;
        target.genome = data.objects.auxiliaryData.getAlignedSubArray(size);
        //#TODO can be optimized
        for (int i = 0; i < size; ++i) {
            target.genome[i] = source.genome[i];
        }
    }
}

__inline__ __device__ bool GenomeDecoder::isSeparating(uint8_t* genome)
{
    return GenomeDecoder::convertByteToBool(genome[Const::GenomeHeaderSeparationPos]);
}

__inline__ __device__ int GenomeDecoder::getNumBranches(uint8_t* genome)
{
    return isSeparating(genome) ? 1 : (genome[Const::GenomeHeaderNumBranchesPos] + 5) % 6 + 1;
}

__inline__ __device__ int GenomeDecoder::getNumRepetitions(uint8_t* genome, bool countInfinityAsOne)
{
    int result = max(1, toInt(genome[Const::GenomeHeaderNumRepetitionsPos]));
    if (!countInfinityAsOne) {
        return result == 255 ? NPP_MAX_32S : result;
    } else {
        return result == 255 ? 1 : result;
    }
}

template <typename ConstructorOrInjector>
__inline__ __device__ bool GenomeDecoder::containsSelfReplication(ConstructorOrInjector const& cellType)
{
    for (int currentNodeAddress = Const::GenomeHeaderSize; currentNodeAddress < cellType.genomeSize;) {
        if (isNextCellSelfReplication(cellType.genome, currentNodeAddress)) {
            return true;
        }
        currentNodeAddress += Const::CellBasicBytes + getNextCellTypeDataSize(cellType.genome, cellType.genomeSize, currentNodeAddress);
    }

    return false;
}

__inline__ __device__ GenomeHeader GenomeDecoder::readGenomeHeader(ConstructorType const& constructor)
{
    CUDA_CHECK(constructor.genomeSize >= Const::GenomeHeaderSize)

    GenomeHeader result;    
    result.shape = constructor.genome[Const::GenomeHeaderShapePos] % ConstructionShape_Count;
    result.numBranches = getNumBranches(constructor.genome);
    result.separateConstruction = isSeparating(constructor.genome);
    result.angleAlignment = constructor.genome[Const::GenomeHeaderAlignmentPos] % ConstructorAngleAlignment_Count;
    result.stiffness = toFloat(constructor.genome[Const::GenomeHeaderStiffnessPos]) / 255;
    result.connectionDistance = toFloat(constructor.genome[Const::GenomeHeaderConstructionDistancePos]) / 255 + 0.5f;
    result.numRepetitions = getNumRepetitions(constructor.genome);
    result.concatenationAngle1 = convertByteToAngle(constructor.genome[Const::GenomeHeaderConcatenationAngle1Pos]);
    result.concatenationAngle2 = convertByteToAngle(constructor.genome[Const::GenomeHeaderConcatenationAngle2Pos]);
    return result;
}

__inline__ __device__ int GenomeDecoder::readWord(uint8_t* genome, int address)
{
    return GenomeDecoder::convertBytesToWord(genome[address], genome[address + 1]);
}

__inline__ __device__ void GenomeDecoder::writeWord(uint8_t* genome, int address, int word)
{
    GenomeDecoder::convertWordToBytes(word, genome[address], genome[address + 1]);
}

__inline__ __device__ float GenomeDecoder::convertByteToFloat(uint8_t b)
{
    return static_cast<float>(static_cast<int8_t>(b)) / 128;
}

__inline__ __device__ uint8_t GenomeDecoder::convertFloatToByte(float v)
{
    return static_cast<uint8_t>(static_cast<int8_t>(v * 128));
}

__inline__ __device__ bool GenomeDecoder::convertByteToBool(uint8_t b)
{
    return static_cast<int8_t>(b) > 0;
}

__inline__ __device__ uint8_t GenomeDecoder::convertBoolToByte(bool value)
{
    return value ? 1 : 0;
}

__inline__ __device__ int GenomeDecoder::convertBytesToWord(uint8_t b1, uint8_t b2)
{
    return static_cast<int>(b1) | (static_cast<int>(b2 << 8));
}

__inline__ __device__ void GenomeDecoder::convertWordToBytes(int word, uint8_t& b1, uint8_t& b2)
{
    b1 = static_cast<uint8_t>(word & 0xff);
    b2 = static_cast<uint8_t>((word >> 8) & 0xff);
}

__inline__ __device__ uint8_t GenomeDecoder::convertAngleToByte(float angle)
{
    if (angle > 180.0f) {
        angle -= 360.0f;
    }
    if (angle < -180.0f) {
        angle += 360.0f;
    }
    return static_cast<uint8_t>(static_cast<int8_t>(angle / 180 * 120));
}

__inline__ __device__ float GenomeDecoder::convertByteToAngle(uint8_t b)
{
    return static_cast<float>(static_cast<int8_t>(b)) / 120 * 180;
}

__inline__ __device__ uint8_t GenomeDecoder::convertOptionalByteToByte(int value)
{
    return static_cast<uint8_t>(value);
}

__inline__ __device__ void GenomeDecoder::setRandomCellTypeData(
    SimulationData& data,
    uint8_t* genome,
    int nodeAddress,
    CellType const& cellType,
    bool makeSelfCopy,
    int subGenomeSize)
{
    auto newCellTypeSize = getCellTypeDataSize(cellType, makeSelfCopy, subGenomeSize);
    data.numberGen1.randomBytes(genome + nodeAddress, newCellTypeSize);
    if (cellType == CellType_Constructor || cellType == CellType_Injector) {
        auto cellTypeFixedBytes = cellType == CellType_Constructor ? Const::ConstructorFixedBytes : Const::InjectorFixedBytes;
        genome[nodeAddress + cellTypeFixedBytes] = makeSelfCopy ? 1 : 0;

        auto subGenomeRelPos = getCellTypeDataSize(cellType, makeSelfCopy, 0);
        genome[nodeAddress + subGenomeRelPos + Const::GenomeHeaderNumRepetitionsPos] = 1;

        if (!makeSelfCopy) {
            writeWord(genome, nodeAddress + cellTypeFixedBytes + 1, subGenomeSize);
        }
    }
}

__inline__ __device__ int GenomeDecoder::getNumNodes(uint8_t* genome, int genomeSize)
{
    int result = 0;
    int currentNodeAddress = Const::GenomeHeaderSize;
    for (; result < genomeSize && currentNodeAddress < genomeSize; ++result) {
        currentNodeAddress += Const::CellBasicBytes + getNextCellTypeDataSize(genome, genomeSize, currentNodeAddress);
    }

    return result;
}

__inline__ __device__ int GenomeDecoder::getNodeAddress(uint8_t* genome, int genomeSize, int nodeIndex)
{
    int currentNodeAddress = Const::GenomeHeaderSize;
    for (int currentNodeIndex = 0; currentNodeIndex < nodeIndex; ++currentNodeIndex) {
        if (currentNodeAddress >= genomeSize) {
            break;
        }
        currentNodeAddress += Const::CellBasicBytes + getNextCellTypeDataSize(genome, genomeSize, currentNodeAddress);
    }

    return currentNodeAddress;
}


__inline__ __device__ int GenomeDecoder::findStartNodeAddress(uint8_t* genome, int genomeSize, int refIndex)
{
    int currentNodeAddress = Const::GenomeHeaderSize;
    for (; currentNodeAddress <= refIndex;) {
        auto prevCurrentNodeAddress = currentNodeAddress;
        currentNodeAddress += Const::CellBasicBytes + getNextCellTypeDataSize(genome, genomeSize, currentNodeAddress);
        if (currentNodeAddress > refIndex) {
            return prevCurrentNodeAddress;
        }
    }
    return Const::GenomeHeaderSize;
}

__inline__ __device__ int GenomeDecoder::getNextCellTypeDataSize(uint8_t* genome, int genomeSize, int nodeAddress, bool withSubgenome)
{
    auto cellType = getNextCellType(genome, nodeAddress);
    switch (cellType) {
    case CellType_Base:
        return Const::BaseBytes;
    case CellType_Depot:
        return Const::TransmitterBytes;
    case CellType_Constructor: {
        if (withSubgenome) {
            auto isMakeCopy = GenomeDecoder::convertByteToBool(genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes]);
            if (isMakeCopy) {
                return Const::ConstructorFixedBytes + 1;
            } else {
                return Const::ConstructorFixedBytes + 3 + getNextSubGenomeSize(genome, genomeSize, nodeAddress);
            }
        } else {
            return Const::ConstructorFixedBytes;
        }
    }
    case CellType_Sensor:
        return Const::SensorBytes;
    case CellType_Oscillator:
        return Const::OscillatorBytes;
    case CellType_Attacker:
        return Const::AttackerBytes;
    case CellType_Injector: {
        if (withSubgenome) {
            auto isMakeCopy = GenomeDecoder::convertByteToBool(genome[nodeAddress + Const::CellBasicBytes + Const::InjectorFixedBytes]);
            if (isMakeCopy) {
                return Const::InjectorFixedBytes + 1;
            } else {
                return Const::InjectorFixedBytes + 3 + getNextSubGenomeSize(genome, genomeSize, nodeAddress);
            }
        } else {
            return Const::InjectorFixedBytes;
        }
    }
    case CellType_Muscle:
        return Const::MuscleBytes;
    case CellType_Defender:
        return Const::DefenderBytes;
    case CellType_Reconnector:
        return Const::ReconnectorBytes;
    case CellType_Detonator:
        return Const::DetonatorBytes;
    default:
        return 0;
    }
}

__inline__ __device__ CellType GenomeDecoder::getNextCellType(uint8_t* genome, int nodeAddress)
{
    return genome[nodeAddress] % CellType_Count;
}

__inline__ __device__ bool GenomeDecoder::isNextCellSelfReplication(uint8_t* genome, int nodeAddress)
{
    switch (getNextCellType(genome, nodeAddress)) {
    case CellType_Constructor:
        return GenomeDecoder::convertByteToBool(genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes]);
    case CellType_Injector:
        return GenomeDecoder::convertByteToBool(genome[nodeAddress + Const::CellBasicBytes + Const::InjectorFixedBytes]);
    }
    return false;
}

__inline__ __device__ int GenomeDecoder::getNextCellColor(uint8_t* genome, int nodeAddress)
{
    return genome[nodeAddress + Const::CellColorPos] % MAX_COLORS;
}

__inline__ __device__ int GenomeDecoder::getNextExecutionNumber(uint8_t* genome, int nodeAddress)
{
    return genome[nodeAddress + Const::CellExecutionNumberPos];
}

__inline__ __device__ int GenomeDecoder::getNextInputExecutionNumber(uint8_t* genome, int nodeAddress)
{
    return genome[nodeAddress + Const::CellInputExecutionNumberPos];
}

__inline__ __device__ void GenomeDecoder::setNextCellType(uint8_t* genome, int nodeAddress, CellType cellType)
{
    genome[nodeAddress] = static_cast<uint8_t>(cellType);
}

__inline__ __device__ void GenomeDecoder::setNextCellSelfReplication(uint8_t* genome, int nodeAddress, bool value)
{
    switch (getNextCellType(genome, nodeAddress)) {
    case CellType_Constructor: {
        genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes] = GenomeDecoder::convertBoolToByte(value);
    } break;
    case CellType_Injector: {
        genome[nodeAddress + Const::CellBasicBytes + Const::InjectorFixedBytes] = GenomeDecoder::convertBoolToByte(value);
    } break;
    }
}

__inline__ __device__ void GenomeDecoder::setNextCellSubgenomeSize(uint8_t* genome, int nodeAddress, int size)
{
    
    switch (getNextCellType(genome, nodeAddress)) {
    case CellType_Constructor: {
        GenomeDecoder::writeWord(genome, nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes + 1, size);
    } break;
    case CellType_Injector: {
        GenomeDecoder::writeWord(genome, nodeAddress + Const::CellBasicBytes + Const::InjectorFixedBytes + 1, size);
    } break;
    }
}

__inline__ __device__ void GenomeDecoder::setNextCellColor(uint8_t* genome, int nodeAddress, int color)
{
    genome[nodeAddress + Const::CellColorPos] = color;
}

__inline__ __device__ void GenomeDecoder::setNextInputExecutionNumber(uint8_t* genome, int nodeAddress, int value)
{
    genome[nodeAddress + Const::CellInputExecutionNumberPos] = value;
}

__inline__ __device__ void GenomeDecoder::setNextOutputBlocked(uint8_t* genome, int nodeAddress, bool value)
{
    genome[nodeAddress + Const::CellOutputBlockedPos] = value ? 1 : 0;
}

__inline__ __device__ void GenomeDecoder::setNextAngle(uint8_t* genome, int nodeAddress, uint8_t angle)
{
    genome[nodeAddress + Const::CellAnglePos] = angle;
}

__inline__ __device__ void GenomeDecoder::setNextRequiredConnections(uint8_t* genome, int nodeAddress, uint8_t angle)
{
    genome[nodeAddress + Const::CellRequiredConnectionsPos] = angle;
}

__inline__ __device__ void GenomeDecoder::setNextConstructionAngle1(uint8_t* genome, int nodeAddress, uint8_t angle)
{
    genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorConstructionAngle1Pos] = angle;
}

__inline__ __device__ void GenomeDecoder::setNextConstructionAngle2(uint8_t* genome, int nodeAddress, uint8_t angle)
{
    genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorConstructionAngle2Pos] = angle;
}

__inline__ __device__ void GenomeDecoder::setNextConstructorSeparation(uint8_t* genome, int nodeAddress, bool separation)
{
    genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes + 3 + Const::GenomeHeaderSeparationPos] = convertBoolToByte(separation);
}

__inline__ __device__ void GenomeDecoder::setNextConstructorNumBranches(uint8_t* genome, int nodeAddress, int numBranches)
{
    genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes + 3 + Const::GenomeHeaderNumBranchesPos] = static_cast<uint8_t>(numBranches);
}

__inline__ __device__ void GenomeDecoder::setNextConstructorNumRepetitions(uint8_t* genome, int nodeAddress, int numRepetitions)
{
    genome[nodeAddress + Const::CellBasicBytes + Const::ConstructorFixedBytes + 3 + Const::GenomeHeaderNumRepetitionsPos] = static_cast<uint8_t>(numRepetitions);
}

__inline__ __device__ int GenomeDecoder::getNextSubGenomeSize(uint8_t* genome, int genomeSize, int nodeAddress)
{
    auto cellType = getNextCellType(genome, nodeAddress);
    auto cellTypeFixedBytes = cellType == CellType_Constructor ? Const::ConstructorFixedBytes : Const::InjectorFixedBytes;
    auto subGenomeSizeIndex = nodeAddress + Const::CellBasicBytes + cellTypeFixedBytes + 1;
    return max(min(GenomeDecoder::convertBytesToWord(genome[subGenomeSizeIndex], genome[subGenomeSizeIndex + 1]), genomeSize - (subGenomeSizeIndex + 2)), 0);
}

__inline__ __device__ int GenomeDecoder::getCellTypeDataSize(CellType cellType, bool makeSelfCopy, int genomeSize)
{
    switch (cellType) {
    case CellType_Base:
        return Const::BaseBytes;
    case CellType_Depot:
        return Const::TransmitterBytes;
    case CellType_Constructor: {
        return makeSelfCopy ? Const::ConstructorFixedBytes + 1 : Const::ConstructorFixedBytes + 3 + genomeSize;
    }
    case CellType_Sensor:
        return Const::SensorBytes;
    case CellType_Oscillator:
        return Const::OscillatorBytes;
    case CellType_Attacker:
        return Const::AttackerBytes;
    case CellType_Injector: {
        return makeSelfCopy ? Const::InjectorFixedBytes + 1 : Const::InjectorFixedBytes + 3 + genomeSize;
    }
    case CellType_Muscle:
        return Const::MuscleBytes;
    case CellType_Defender:
        return Const::DefenderBytes;
    case CellType_Reconnector:
        return Const::ReconnectorBytes;
    case CellType_Detonator:
        return Const::DetonatorBytes;
    default:
        return 0;
    }
}

__inline__ __device__ bool GenomeDecoder::containsSectionSelfReplication(uint8_t* genome, int genomeSize)
{
    int nodeAddress = 0;
    for (; nodeAddress < genomeSize;) {
        if (isNextCellSelfReplication(genome, nodeAddress)) {
            return true;
        }
        nodeAddress += Const::CellBasicBytes + getNextCellTypeDataSize(genome, genomeSize, nodeAddress);
    }

    return false;
}

__inline__ __device__ int GenomeDecoder::getNodeAddressForSelfReplication(uint8_t* genome, int genomeSize, bool& containsSelfReplicator)
{
    int nodeAddress = 0;
    for (; nodeAddress < genomeSize;) {
        if (isNextCellSelfReplication(genome, nodeAddress)) {
            containsSelfReplicator = true;
            return nodeAddress;
        }
        nodeAddress += Const::CellBasicBytes + getNextCellTypeDataSize(genome, genomeSize, nodeAddress);
    }
    containsSelfReplicator = false;
    return 0;
}
