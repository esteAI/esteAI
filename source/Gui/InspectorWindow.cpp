#include "InspectorWindow.h"

#include <sstream>
#include <imgui.h>

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include "EngineInterface/DescriptionEditService.h"
#include "EngineInterface/SimulationFacade.h"
#include "EngineInterface/GenomeDescriptionService.h"
#include "EngineInterface/PreviewDescriptionService.h"

#include "StyleRepository.h"
#include "Viewport.h"
#include "EditorModel.h"
#include "AlienImGui.h"
#include "CellTypeStrings.h"
#include "GenomeEditorWindow.h"
#include "HelpStrings.h"
#include "OverlayController.h"

using namespace std::string_literals;

namespace
{
    auto const CellWindowWidth = 380.0f;
    auto const ParticleWindowWidth = 280.0f;
    auto const BaseTabTextWidth = 162.0f;
    auto const CellTypeTextWidth = 195.0f;
    auto const CellTypeDefenderWidth = 100.0f;
    auto const CellTypeBaseTabTextWidth = 150.0f;
    auto const SignalTextWidth = 130.0f;
    auto const GenomeTabTextWidth = 195.0f;
    auto const ParticleContentTextWidth = 80.0f;

    auto const TreeNodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
}

_InspectorWindow::_InspectorWindow(SimulationFacade const& simulationFacade, uint64_t entityId, RealVector2D const& initialPos, bool selectGenomeTab)
    : _entityId(entityId)
    , _initialPos(initialPos)
    , _simulationFacade(simulationFacade)
    , _selectGenomeTab(selectGenomeTab)
{
}

_InspectorWindow::~_InspectorWindow() {}

void _InspectorWindow::process()
{
    if (!_on) {
        return;
    }
    auto width = calcWindowWidth();
    auto height = isCell() ? StyleRepository::get().scale(370.0f)
                           : StyleRepository::get().scale(70.0f);
    auto borderlessRendering = _simulationFacade->getSimulationParameters().borderlessRendering;
    ImGui::SetNextWindowBgAlpha(Const::WindowAlpha * ImGui::GetStyle().Alpha);
    ImGui::SetNextWindowSize({width, height}, ImGuiCond_Appearing);
    ImGui::SetNextWindowPos({_initialPos.x, _initialPos.y}, ImGuiCond_Appearing);
    auto entity = EditorModel::get().getInspectedEntity(_entityId);
    if (ImGui::Begin(generateTitle().c_str(), &_on, ImGuiWindowFlags_HorizontalScrollbar)) {
        auto windowPos = ImGui::GetWindowPos();
        if (isCell()) {
            processCell(std::get<CellDescription>(entity));
        } else {
            processParticle(std::get<ParticleDescription>(entity));
        }
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        auto entityPos = Viewport::get().mapWorldToViewPosition(DescriptionEditService::get().getPos(entity), borderlessRendering);
        auto factor = StyleRepository::get().scale(1);

        drawList->AddLine(
            {windowPos.x + 15.0f * factor, windowPos.y - 5.0f * factor},
            {entityPos.x, entityPos.y},
            Const::InspectorLineColor,
            1.5f);
        drawList->AddRectFilled(
            {windowPos.x + 5.0f * factor, windowPos.y - 10.0f * factor},
            {windowPos.x + 25.0f * factor, windowPos.y},
            Const::InspectorRectColor,
            1.0,
            0);
        drawList->AddRect(
            {windowPos.x + 5.0f * factor, windowPos.y - 10.0f * factor},
            {windowPos.x + 25.0f * factor, windowPos.y},
            Const::InspectorLineColor,
            1.0,
            0,
            2.0f);
    }
    ImGui::End();
}

bool _InspectorWindow::isClosed() const
{
    return !_on;
}

uint64_t _InspectorWindow::getId() const
{
    return _entityId;
}

bool _InspectorWindow::isCell() const
{
    auto entity = EditorModel::get().getInspectedEntity(_entityId);
    return std::holds_alternative<CellDescription>(entity);
}

std::string _InspectorWindow::generateTitle() const
{
    auto entity = EditorModel::get().getInspectedEntity(_entityId);
    std::stringstream ss;
    if (isCell()) {
        ss << "Cell with id 0x" << std::hex << std::uppercase << _entityId;
    } else {
        ss << "Energy particle with id 0x" << std::hex << std::uppercase << _entityId;
    }
    return ss.str();
}

void _InspectorWindow::processCell(CellDescription cell)
{
    if (ImGui::BeginTabBar(
            "##CellInspect", /*ImGuiTabBarFlags_AutoSelectNewTabs | */ImGuiTabBarFlags_FittingPolicyResizeDown)) {
        auto origCell = cell;
        processCellBaseTab(cell);
        processCellTypeTab(cell);
        processCellTypePropertiesTab(cell);
        if (cell.getCellType() == CellType_Constructor) {
            processCellGenomeTab(std::get<ConstructorDescription>(cell.cellTypeData));
        }
        if (cell.getCellType() == CellType_Injector) {
            processCellGenomeTab(std::get<InjectorDescription>(cell.cellTypeData));
        }
        processCellMetadataTab(cell);
        validateAndCorrect(cell);

        ImGui::EndTabBar();

        if (cell != origCell) {
            _simulationFacade->changeCell(cell);
        }
    }
}

void _InspectorWindow::processCellBaseTab(CellDescription& cell)
{
    if (ImGui::BeginTabItem("Base", nullptr, ImGuiTabItemFlags_None)) {
        if (ImGui::BeginChild("##", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            if (ImGui::TreeNodeEx("Properties##Base", TreeNodeFlags)) {
                std::stringstream ss;
                ss << "0x" << std::hex << std::uppercase << cell.id;
                auto cellId = ss.str();

                AlienImGui::ComboColor(
                    AlienImGui::ComboColorParameters().name("Color").textWidth(BaseTabTextWidth).tooltip(Const::GenomeColorTooltip), cell.color);
                AlienImGui::InputFloat(
                    AlienImGui::InputFloatParameters().name("Energy").format("%.2f").textWidth(BaseTabTextWidth).tooltip(Const::CellEnergyTooltip),
                    cell.energy);
                AlienImGui::InputInt(AlienImGui::InputIntParameters().name("Age").textWidth(BaseTabTextWidth).tooltip(Const::CellAgeTooltip), cell.age);
                AlienImGui::InputFloat(AlienImGui::InputFloatParameters().name("Position X").format("%.2f").textWidth(BaseTabTextWidth), cell.pos.x);
                AlienImGui::InputFloat(AlienImGui::InputFloatParameters().name("Position Y").format("%.2f").textWidth(BaseTabTextWidth), cell.pos.y);
                AlienImGui::InputFloat(AlienImGui::InputFloatParameters().name("Velocity X").format("%.2f").textWidth(BaseTabTextWidth), cell.vel.x);
                AlienImGui::InputFloat(AlienImGui::InputFloatParameters().name("Velocity Y").format("%.2f").textWidth(BaseTabTextWidth), cell.vel.y);
                AlienImGui::InputFloat(
                    AlienImGui::InputFloatParameters().name("Stiffness").format("%.2f").step(0.05f).textWidth(BaseTabTextWidth).tooltip(Const::CellStiffnessTooltip),
                    cell.stiffness);
                AlienImGui::Checkbox(
                    AlienImGui::CheckboxParameters().name("Indestructible wall").textWidth(BaseTabTextWidth).tooltip(Const::CellIndestructibleTooltip), cell.barrier);
                AlienImGui::InputText(
                    AlienImGui::InputTextParameters().name("Cell id").textWidth(BaseTabTextWidth).tooltip(Const::CellIdTooltip).readOnly(true), cellId);
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Associated creature##Base", TreeNodeFlags)) {
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters().name("Creature id").textWidth(BaseTabTextWidth).tooltip(Const::CellCreatureIdTooltip), cell.creatureId);
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters().name("Mutation id").textWidth(BaseTabTextWidth).tooltip(Const::CellMutationIdTooltip), cell.mutationId);
                AlienImGui::InputFloat(
                    AlienImGui::InputFloatParameters().name("Genome complexity").textWidth(BaseTabTextWidth).tooltip(Const::GenomeComplexityTooltip),
                    cell.genomeComplexity);

                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("Connections to other cells", TreeNodeFlags)) {
                for (auto const& [index, connection] : cell.connections | boost::adaptors::indexed(0)) {
                    if (ImGui::TreeNodeEx(("Connection [" + std::to_string(index) + "]").c_str(), ImGuiTreeNodeFlags_None)) {
                        std::stringstream ss;
                        ss << "0x" << std::hex << std::uppercase << connection.cellId;
                        auto cellId = ss.str();

                        AlienImGui::InputText(
                            AlienImGui::InputTextParameters().name("Cell id").textWidth(BaseTabTextWidth).tooltip(Const::CellIdTooltip).readOnly(true),
                            cellId);
                        AlienImGui::InputFloat(
                            AlienImGui::InputFloatParameters()
                                .name("Reference distance")
                                .format("%.2f")
                                .textWidth(BaseTabTextWidth)
                                .readOnly(true)
                                .tooltip(Const::CellReferenceDistanceTooltip),
                            connection.distance);
                        AlienImGui::InputFloat(
                            AlienImGui::InputFloatParameters()
                                .name("Reference angle")
                                .format("%.2f")
                                .textWidth(BaseTabTextWidth)
                                .readOnly(true)
                                .tooltip(Const::CellReferenceAngleTooltip),
                            connection.angleFromPrevious);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void _InspectorWindow::processCellTypeTab(CellDescription& cell)
{
    if (ImGui::BeginTabItem("Function", nullptr, ImGuiTabItemFlags_None)) {
        int type = cell.getCellType();
        if (ImGui::BeginChild("##", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {

            if (cell.neuralNetwork.has_value()) {
                processNeuronContent(cell);
            }

            if (ImGui::TreeNodeEx("Properties##Function", TreeNodeFlags)) {
                if (AlienImGui::CellTypeCombo(
                        AlienImGui::CellTypeComboParameters()
                            .name("Function")
                            .textWidth(CellTypeBaseTabTextWidth)
                            .includeStructureAndFreeCells(true)
                            .tooltip(Const::getCellTypeTooltip(type)),
                        type)) {
                    switch (type) {
                    case CellType_Structure: {
                        cell.cellTypeData = StructureCellDescription();
                    } break;
                    case CellType_Free: {
                        cell.cellTypeData = FreeCellDescription();
                    } break;
                    case CellType_Base: {
                        cell.cellTypeData = BaseDescription();
                    } break;
                    case CellType_Depot: {
                        cell.cellTypeData = DepotDescription();
                    } break;
                    case CellType_Constructor: {
                        cell.cellTypeData = ConstructorDescription();
                    } break;
                    case CellType_Sensor: {
                        cell.cellTypeData = SensorDescription();
                    } break;
                    case CellType_Oscillator: {
                        cell.cellTypeData = OscillatorDescription();
                    } break;
                    case CellType_Attacker: {
                        cell.cellTypeData = AttackerDescription();
                    } break;
                    case CellType_Injector: {
                        cell.cellTypeData = InjectorDescription();
                    } break;
                    case CellType_Muscle: {
                        cell.cellTypeData = MuscleDescription();
                    } break;
                    case CellType_Defender: {
                        cell.cellTypeData = DefenderDescription();
                    } break;
                    case CellType_Reconnector: {
                        cell.cellTypeData = ReconnectorDescription();
                    } break;
                    case CellType_Detonator: {
                        cell.cellTypeData = DetonatorDescription();
                    } break;
                    }
                }

                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters()
                        .name("Activation time")
                        .textWidth(CellTypeBaseTabTextWidth)
                        .tooltip(Const::GenomeConstructorOffspringActivationTime),
                    cell.activationTime);
                AlienImGui::Combo(
                    AlienImGui::ComboParameters()
                        .name("Living state")
                        .textWidth(CellTypeBaseTabTextWidth)
                        .values({"Ready", "Under construction", "Activating", "Detached", "Reviving", "Dying"})
                        .tooltip(Const::CellLivingStateTooltip),
                    cell.livingState);
                ImGui::TreePop();
            }
        }
        if (cell.signal.has_value()) {
            if (ImGui::TreeNodeEx("Signals", TreeNodeFlags)) {
                int index = 0;
                for (auto& channel : cell.signal->channels) {
                    AlienImGui::InputFloat(
                        AlienImGui::InputFloatParameters().name("Channel #" + std::to_string(index)).format("%.3f").step(0.1f).textWidth(SignalTextWidth),
                        channel);
                    ++index;
                }
                ImGui::TreePop();
            }
        }

        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void _InspectorWindow::processCellTypePropertiesTab(CellDescription& cell)
{
    if (cell.getCellType() == CellType_Structure || cell.getCellType() == CellType_Free) {
        return;
    }

    std::string title = Const::CellTypeToStringMap.at(cell.getCellType());
    if (ImGui::BeginTabItem(title.c_str(), nullptr, ImGuiTabItemFlags_None)) {
        if (ImGui::BeginChild("##", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            switch (cell.getCellType()) {
            case CellType_Base: {
            } break;
            case CellType_Depot: {
                processTransmitterContent(std::get<DepotDescription>(cell.cellTypeData));
            } break;
            case CellType_Constructor: {
                processConstructorContent(std::get<ConstructorDescription>(cell.cellTypeData));
            } break;
            case CellType_Sensor: {
                processSensorContent(std::get<SensorDescription>(cell.cellTypeData));
            } break;
            case CellType_Oscillator: {
                processOscillatorContent(std::get<OscillatorDescription>(cell.cellTypeData));
            } break;
            case CellType_Attacker: {
                processAttackerContent(std::get<AttackerDescription>(cell.cellTypeData));
            } break;
            case CellType_Injector: {
                processInjectorContent(std::get<InjectorDescription>(cell.cellTypeData));
            } break;
            case CellType_Muscle: {
                processMuscleContent(std::get<MuscleDescription>(cell.cellTypeData));
            } break;
            case CellType_Defender: {
                processDefenderContent(std::get<DefenderDescription>(cell.cellTypeData));
            } break;
            case CellType_Reconnector: {
                processReconnectorContent(std::get<ReconnectorDescription>(cell.cellTypeData));
            } break;
            case CellType_Detonator: {
                processDetonatorContent(std::get<DetonatorDescription>(cell.cellTypeData));
            } break;
            }
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

template <typename Description>
void _InspectorWindow::processCellGenomeTab(Description& desc)
{
    auto const& parameters = _simulationFacade->getSimulationParameters();

    int flags = ImGuiTabItemFlags_None;
    if (_selectGenomeTab) {
        flags = flags | ImGuiTabItemFlags_SetSelected;
        _selectGenomeTab = false;
    }
    if (ImGui::BeginTabItem("Genome", nullptr, flags)) {
        if (ImGui::BeginChild("##", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {

            auto previewNodeResult = ImGui::TreeNodeEx("Preview (reference configuration)", TreeNodeFlags);
            AlienImGui::HelpMarker(Const::GenomePreviewTooltip);
            if (previewNodeResult) {
                if (ImGui::BeginChild("##child", ImVec2(0, scale(200)), true, ImGuiWindowFlags_HorizontalScrollbar)) {
                    auto genomDesc = GenomeDescriptionService::get().convertBytesToDescription(desc.genome);
                    auto previewDesc = PreviewDescriptionService::get().convert(genomDesc, std::nullopt, parameters);
                    std::optional<int> selectedNodeDummy;
                    AlienImGui::ShowPreviewDescription(previewDesc, _genomeZoom, selectedNodeDummy);
                }
                ImGui::EndChild();
                if (AlienImGui::Button("Edit")) {
                    GenomeEditorWindow::get().openTab(GenomeDescriptionService::get().convertBytesToDescription(desc.genome));
                }

                ImGui::SameLine();
                if (AlienImGui::Button(AlienImGui::ButtonParameters().buttonText("Inject from editor").textWidth(ImGui::GetContentRegionAvail().x))) {
                    printOverlayMessage("Genome injected");
                    desc.genome = GenomeDescriptionService::get().convertDescriptionToBytes(GenomeEditorWindow::get().getCurrentGenome());
                    if constexpr (std::is_same<Description, ConstructorDescription>()) {
                        desc.genomeCurrentNodeIndex = 0;
                        desc.setNumInheritedGenomeNodes(0);
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Properties (entire genome)", TreeNodeFlags)) {
                auto numNodes = toInt(GenomeDescriptionService::get().getNumNodesRecursively(desc.genome, true));
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters()
                        .name("Number of cells")
                        .textWidth(GenomeTabTextWidth)
                        .readOnly(true)
                        .tooltip(Const::GenomeNumCellsRecursivelyTooltip),
                    numNodes);

                auto numBytes = toInt(desc.genome.size());
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters()
                        .name("Bytes")
                        .textWidth(GenomeTabTextWidth)
                        .readOnly(true)
                        .tooltip(Const::GenomeBytesTooltip),
                    numBytes);

                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters().name("Generation").textWidth(GenomeTabTextWidth).tooltip(Const::GenomeGenerationTooltip),
                    desc.genomeGeneration);
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("Properties (principal genome part)", TreeNodeFlags)) {

                auto genomeDesc = GenomeDescriptionService::get().convertBytesToDescription(desc.genome);
                auto numBranches = genomeDesc.header.getNumBranches();
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters()
                        .name("Number of branches")
                        .textWidth(GenomeTabTextWidth)
                        .readOnly(true)
                        .tooltip(Const::GenomeNumBranchesTooltip),
                    numBranches);

                auto numRepetitions = genomeDesc.header.numRepetitions;
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters()
                        .name("Repetitions per branch")
                        .textWidth(GenomeTabTextWidth)
                        .infinity(true)
                        .readOnly(true)
                        .tooltip(Const::GenomeRepetitionsPerBranchTooltip),
                    numRepetitions);

                auto numNodes = toInt(genomeDesc.cells.size());
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters()
                        .name("Cells per repetition")
                        .textWidth(GenomeTabTextWidth)
                        .readOnly(true)
                        .tooltip(Const::GenomeNumCellsTooltip),
                    numNodes);

                if constexpr (std::is_same<Description, ConstructorDescription>()) {
                    AlienImGui::InputInt(
                        AlienImGui::InputIntParameters()
                            .name("Current branch index")
                            .textWidth(GenomeTabTextWidth).tooltip(Const::GenomeCurrentBranchTooltip),
                        desc.currentBranch);
                    AlienImGui::InputInt(
                        AlienImGui::InputIntParameters()
                            .name("Current repetition index")
                            .textWidth(GenomeTabTextWidth)
                            .tooltip(Const::GenomeCurrentRepetitionTooltip),
                        desc.genomeCurrentRepetition);
                    AlienImGui::InputInt(
                        AlienImGui::InputIntParameters().name("Current cell index").textWidth(GenomeTabTextWidth).tooltip(Const::GenomeCurrentCellTooltip),
                        desc.genomeCurrentNodeIndex);
                }
                ImGui::TreePop();
            }
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void _InspectorWindow::processCellMetadataTab(CellDescription& cell)
{
    if (ImGui::BeginTabItem("Annotation", nullptr, ImGuiTabItemFlags_None)) {
        if (ImGui::BeginChild("##", ImVec2(0, 0), false, 0)) {
            AlienImGui::InputText(AlienImGui::InputTextParameters().hint("Name").textWidth(0), cell.metadata.name);

            AlienImGui::InputTextMultiline(AlienImGui::InputTextMultilineParameters().hint("Notes").textWidth(0).height(100), cell.metadata.description);
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
    }
}

void _InspectorWindow::processOscillatorContent(OscillatorDescription& oscillator)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {

        bool pulseGeneration = oscillator.pulseMode > 0;
        if (AlienImGui::Checkbox(AlienImGui::CheckboxParameters().name("Generate pulses").textWidth(CellTypeTextWidth).tooltip(Const::GenomeOscillatorGeneratePulsesTooltip), pulseGeneration)) {
            oscillator.pulseMode = pulseGeneration ? 1 : 0;
        }
        if (pulseGeneration) {
            AlienImGui::InputInt(AlienImGui::InputIntParameters().name("Pulse interval").textWidth(CellTypeTextWidth).tooltip(Const::GenomeOscillatorPulseIntervalTooltip), oscillator.pulseMode);
            bool alternation = oscillator.alternationMode > 0;
            if (AlienImGui::Checkbox(
                    AlienImGui::CheckboxParameters().name("Alternating pulses").textWidth(CellTypeTextWidth).tooltip(Const::GenomeOscillatorAlternatingPulsesTooltip),
                    alternation)) {
                oscillator.alternationMode = alternation ? 1 : 0;
            }
            if (alternation) {
                AlienImGui::InputInt(
                    AlienImGui::InputIntParameters().name("Pulses per phase").textWidth(CellTypeTextWidth).tooltip(Const::GenomeOscillatorPulsesPerPhaseTooltip),
                    oscillator.alternationMode);
            }
        }
        ImGui::TreePop();
    }
}

void _InspectorWindow::processNeuronContent(CellDescription& cell)
{
    if (ImGui::TreeNodeEx("Neural network", TreeNodeFlags)) {
        AlienImGui::NeuronSelection(
            AlienImGui::NeuronSelectionParameters().rightMargin(0),
            cell.neuralNetwork->weights,
            cell.neuralNetwork->biases,
            cell.neuralNetwork->activationFunctions);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processConstructorContent(ConstructorDescription& constructor)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        int constructorMode = constructor.activationMode == 0 ? 0 : 1;
        if (AlienImGui::Combo(
                AlienImGui::ComboParameters()
                    .name("Activation mode")
                    .textWidth(CellTypeTextWidth)
                    .values({"Manual", "Automatic"})
                    .tooltip(Const::GenomeConstructorActivationModeTooltip),
                constructorMode)) {
            constructor.activationMode = constructorMode;
        }
        if (constructorMode == 1) {
            AlienImGui::InputInt(
                AlienImGui::InputIntParameters().name("Interval").textWidth(CellTypeTextWidth).tooltip(Const::GenomeConstructorIntervalTooltip),
                constructor.activationMode);
        }
        AlienImGui::InputInt(
            AlienImGui::InputIntParameters()
                .name("Offspring activation time")
                .textWidth(CellTypeTextWidth)
                .tooltip(Const::GenomeConstructorOffspringActivationTime),
            constructor.constructionActivationTime);
        AlienImGui::InputFloat(
            AlienImGui::InputFloatParameters()
                .name("Construction angle #1")
                .textWidth(CellTypeTextWidth)
                .format("%.1f")
                .tooltip(Const::GenomeConstructorAngle1Tooltip),
            constructor.constructionAngle1);
        AlienImGui::InputFloat(
            AlienImGui::InputFloatParameters()
                .name("Construction angle #2")
                .textWidth(CellTypeTextWidth)
                .format("%.1f")
                .tooltip(Const::GenomeConstructorAngle2Tooltip),
            constructor.constructionAngle2);
        ImGui::TreePop();

    }
}

void _InspectorWindow::processInjectorContent(InjectorDescription& injector)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Mode")
                .textWidth(CellTypeTextWidth)
                .values({"Only empty cells", "All cells"})
                .tooltip(Const::GenomeInjectorModeTooltip),
            injector.mode);
        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("Process data", TreeNodeFlags)) {
        AlienImGui::InputInt(
            AlienImGui::InputIntParameters().name("Counter").textWidth(CellTypeTextWidth).tooltip(Const::CellInjectorCounterTooltip), injector.counter);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processAttackerContent(AttackerDescription& attacker)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Energy distribution")
                .values({"Connected cells", "Transmitters and Constructors"}).tooltip(Const::GenomeAttackerEnergyDistributionTooltip)
                .textWidth(CellTypeTextWidth),
            attacker.mode);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processDefenderContent(DefenderDescription& defender)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Mode")
                .values({"Anti-attacker", "Anti-injector"})
                .textWidth(CellTypeDefenderWidth)
                .tooltip(Const::GenomeDefenderModeTooltip),
            defender.mode);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processTransmitterContent(DepotDescription& transmitter)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Energy distribution")
                .values({"Connected cells", "Transmitters and Constructors"})
                .tooltip(Const::GenomeTransmitterEnergyDistributionTooltip)
                .textWidth(CellTypeTextWidth),
            transmitter.mode);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processMuscleContent(MuscleDescription& muscle)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Mode")
                .values({"Movement to sensor target", "Expansion and contraction", "Bending"})
                .textWidth(CellTypeTextWidth)
                .tooltip(Const::GenomeMuscleModeTooltip),
            muscle.mode);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processSensorContent(SensorDescription& sensor)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::ComboOptionalColor(
            AlienImGui::ComboColorParameters().name("Scan color").textWidth(CellTypeTextWidth).tooltip(Const::GenomeSensorScanColorTooltip), sensor.restrictToColor);

        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Scan mutants")
                .values({"None", "Same mutants", "Other mutants", "Free cells", "Handcrafted cells", "Less complex mutants", "More complex mutants"})
                .textWidth(CellTypeTextWidth)
                .tooltip(Const::SensorRestrictToMutantsTooltip),
            sensor.restrictToMutants);
        AlienImGui::InputFloat(
            AlienImGui::InputFloatParameters()
                .name("Min density")
                .format("%.2f")
                .step(0.05f)
                .textWidth(CellTypeTextWidth)
                .tooltip(Const::GenomeSensorMinDensityTooltip),
            sensor.minDensity);
        AlienImGui::InputOptionalInt(
            AlienImGui::InputIntParameters().name("Min range").textWidth(CellTypeTextWidth).tooltip(Const::GenomeSensorMinRangeTooltip), sensor.minRange);
        AlienImGui::InputOptionalInt(
            AlienImGui::InputIntParameters().name("Max range").textWidth(CellTypeTextWidth).tooltip(Const::GenomeSensorMaxRangeTooltip), sensor.maxRange);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processReconnectorContent(ReconnectorDescription& reconnector)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::ComboOptionalColor(
            AlienImGui::ComboColorParameters().name("Restrict to color").textWidth(CellTypeTextWidth).tooltip(Const::GenomeReconnectorRestrictToColorTooltip),
            reconnector.restrictToColor);
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("Restrict to mutants")
                .values({"None", "Same mutants", "Other mutants", "Free cells", "Handcrafted cells", "Less complex mutants", "More complex mutants"})
                .textWidth(CellTypeTextWidth)
                .tooltip(Const::ReconnectorRestrictToMutantsTooltip),
            reconnector.restrictToMutants);

        ImGui::TreePop();
    }
}

void _InspectorWindow::processDetonatorContent(DetonatorDescription& detonator)
{
    if (ImGui::TreeNodeEx("Properties", TreeNodeFlags)) {
        AlienImGui::Combo(
            AlienImGui::ComboParameters()
                .name("State")
                .values({"Ready", "Activated", "Exploded"})
                .textWidth(CellTypeTextWidth)
                .tooltip(Const::DetonatorStateTooltip),
            detonator.state);

        AlienImGui::InputInt(
            AlienImGui::InputIntParameters().name("Countdown").textWidth(CellTypeTextWidth).tooltip(Const::GenomeDetonatorCountdownTooltip),
            detonator.countdown);
        ImGui::TreePop();
    }
}

void _InspectorWindow::processParticle(ParticleDescription particle)
{
    auto origParticle = particle;
    auto energy = toFloat(particle.energy);
    AlienImGui::InputFloat(AlienImGui::InputFloatParameters().name("Energy").textWidth(ParticleContentTextWidth), energy);

    particle.energy = energy;
    if (particle != origParticle) {
        _simulationFacade->changeParticle(particle);
    }
}

float _InspectorWindow::calcWindowWidth() const
{
    if (isCell()) {
        return StyleRepository::get().scale(CellWindowWidth);
    } else {
        return StyleRepository::get().scale(ParticleWindowWidth);
    }
}

void _InspectorWindow::validateAndCorrect(CellDescription& cell) const
{
    auto const& parameters = _simulationFacade->getSimulationParameters();

    cell.stiffness = std::max(0.0f, std::min(1.0f, cell.stiffness));
    cell.energy = std::max(0.0f, cell.energy);
    switch (cell.getCellType()) {
    case CellType_Constructor: {
        auto& constructor = std::get<ConstructorDescription>(cell.cellTypeData);
        auto numNodes = GenomeDescriptionService::get().convertNodeAddressToNodeIndex(constructor.genome, toInt(constructor.genome.size()));
        if (numNodes > 0) {
            constructor.genomeCurrentNodeIndex = ((constructor.genomeCurrentNodeIndex % numNodes) + numNodes) % numNodes;
        } else {
            constructor.genomeCurrentNodeIndex = 0;
        }

        auto numRepetitions = GenomeDescriptionService::get().getNumRepetitions(constructor.genome);
        if (numRepetitions != std::numeric_limits<int>::max()) {
            constructor.genomeCurrentRepetition = ((constructor.genomeCurrentRepetition % numRepetitions) + numRepetitions) % numRepetitions;
        } else {
            constructor.genomeCurrentRepetition = 0;
        }

        constructor.constructionActivationTime = ((constructor.constructionActivationTime % Const::MaxActivationTime) + Const::MaxActivationTime) % Const::MaxActivationTime;
        if (constructor.constructionActivationTime < 0) {
            constructor.constructionActivationTime = 0;
        }
        if (constructor.activationMode < 0) {
            constructor.activationMode = 0;
        }
        constructor.genomeGeneration = std::max(0, constructor.genomeGeneration);
    } break;
    case CellType_Sensor: {
        auto& sensor = std::get<SensorDescription>(cell.cellTypeData);
        sensor.minDensity = std::max(0.0f, std::min(1.0f, sensor.minDensity));
        if (sensor.minRange) {
            sensor.minRange = std::max(0, std::min(127, *sensor.minRange));
        }
        if (sensor.maxRange) {
            sensor.maxRange = std::max(0, std::min(127, *sensor.maxRange));
        }
    } break;
    case CellType_Oscillator: {
        auto& oscillator = std::get<OscillatorDescription>(cell.cellTypeData);
        oscillator.pulseMode = std::max(0, oscillator.pulseMode);
        oscillator.alternationMode = std::max(0, oscillator.alternationMode);
    } break;
    case CellType_Detonator: {
        auto& detonator = std::get<DetonatorDescription>(cell.cellTypeData);
        detonator.countdown = std::min(65535, std::max(0, detonator.countdown));
    } break;
    }
}
