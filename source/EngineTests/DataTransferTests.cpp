#include <gtest/gtest.h>

#include "Base/NumberGenerator.h"
#include "EngineInterface/DescriptionEditService.h"
#include "EngineInterface/Descriptions.h"
#include "EngineInterface/SimulationFacade.h"
#include "IntegrationTestFramework.h"

class DataTransferTests : public IntegrationTestFramework
{
public:
    DataTransferTests()
        : IntegrationTestFramework()
    {}

    ~DataTransferTests() = default;
};

TEST_F(DataTransferTests, singleCell)
{
    DataDescription data;
    NeuralNetworkDescription nn;
    nn.setWeight(2, 1, 1.0f);
    data.addCell(CellDescription()
                     .setNeuralNetwork(nn)
                     .setId(1)
                     .setPos({2.0f, 4.0f})
                     .setVel({0.5f, 1.0f})
                     .setEnergy(100.0f)
                     .setAge(1)
                     .setColor(2)
                     .setBarrier(true)
                     .setLivingState(false)
                     .setConstructionId(3534)
                     .setSignal({1, 0, -1, 0, 0, 0, 0, 0}));

    _simulationFacade->setSimulationData(data);
    auto actualData = _simulationFacade->getSimulationData();

    EXPECT_TRUE(compare(data, actualData));
}

TEST_F(DataTransferTests, singleParticle)
{
    DataDescription data;

    data.addParticle(ParticleDescription().setId(1).setPos({2.0f, 4.0f}).setVel({0.5f, 1.0f}).setEnergy(100.0f).setColor(2));

    _simulationFacade->setSimulationData(data);
    auto actualData = _simulationFacade->getSimulationData();

    EXPECT_TRUE(compare(data, actualData));
}

TEST_F(DataTransferTests, cellCluster)
{
    NeuralNetworkDescription nn1;
    nn1.setWeight(2, 1, 1.0f);
    NeuralNetworkDescription nn2;
    nn2.setWeight(5, 3, 1.0f);

    DataDescription data;
    data.addCells({
        CellDescription().setId(1).setPos({2.0f, 4.0f}).setVel({0.5f, 1.0f}).setAge(1).setColor(2).setBarrier(false).setLivingState(false).setNeuralNetwork(nn1),
        CellDescription().setId(2).setPos({3.0f, 4.0f}).setVel({0.2f, 1.0f}).setAge(1).setColor(4).setBarrier(true).setLivingState(false).setNeuralNetwork(nn2),
    });
    data.addConnection(1, 2);

    _simulationFacade->setSimulationData(data);
    auto actualData = _simulationFacade->getSimulationData();

    EXPECT_TRUE(compare(data, actualData));
}

TEST_F(DataTransferTests, largeData)
{
    auto& numberGen = NumberGenerator::get();
    auto addCellAndParticles = [&](DataDescription& data) {
        data.addCell(CellDescription()
                         .setId(numberGen.getId())
                .setPos({numberGen.getRandomFloat(0.0f, 100.0f), numberGen.getRandomFloat(0.0f, 100.0f)})
                         .setVel({numberGen.getRandomFloat(-1.0f, 1.0f), numberGen.getRandomFloat(-1.0f, 1.0f)})
                        .setEnergy(numberGen.getRandomFloat(0.0f, 100.0f))
                         .setAge(1)
                         .setColor(2)
                         .setBarrier(true)
                         .setLivingState(false));
        data.addParticle(ParticleDescription()
                             .setId(numberGen.getId())
                             .setPos({numberGen.getRandomFloat(0.0f, 100.0f), numberGen.getRandomFloat(0.0f, 100.0f)})
                             .setVel({numberGen.getRandomFloat(-1.0f, 1.0f), numberGen.getRandomFloat(-1.0f, 1.0f)})
                             .setEnergy(numberGen.getRandomFloat(0.0f, 100.0f)));
    };

    DataDescription data;
    for (int i = 0; i < 100000; ++i) {
        addCellAndParticles(data);
    }
    {
        _simulationFacade->setSimulationData(data);
        auto actualData = _simulationFacade->getSimulationData();
        EXPECT_TRUE(compare(data, actualData));
    }

    DataDescription newData;
    for (int i = 0; i < 1000000; ++i) {
        addCellAndParticles(newData);
    }
    {
        _simulationFacade->addAndSelectSimulationData(newData);
        auto actualData = _simulationFacade->getSimulationData();
        EXPECT_EQ(data.cells.size() + newData.cells.size(), actualData.cells.size());
        EXPECT_EQ(data.particles.size() + newData.particles.size(), actualData.particles.size());
    }
}
