#include <functional>
#include <atomic>
#include <vector>

#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterRootTree.h"

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC3.h"

#include "tbb/concurrent_queue.h"

#include "profile.h"

using namespace HepMC3;

class HepMCWriter
{
public:
    HepMCWriter() 
    {
        m_isActive = true;
        m_workerThread = std::thread(&HepMCWriter::workerThread, this);
    }

    void writeEvent(GenEvent* eventPtr)
    {
        m_taskQueue.emplace([this, eventPtr]() { this->i_writeEvent(eventPtr); });
    }

    ~HepMCWriter()
    {
        m_taskQueue.emplace([this]() { this->m_isActive = false; });
        m_workerThread.join();
        m_writer.close();
    }

private:
    void i_writeEvent(GenEvent* eventPtr)
    {
        m_writer.write_event(*eventPtr);
        delete eventPtr;
    }

    void workerThread()
    {
        std::function<void()> task;
        while (m_isActive)
        {
            m_taskQueue.pop(task);
            task();
        }
    }

    tbb::concurrent_bounded_queue<std::function<void()>> m_taskQueue;
    WriterRootTree m_writer{"out.root"};
    std::thread m_workerThread;
    std::atomic<bool> m_isActive;
};



void generatorTread(HepMCWriter& w)
{
    Pythia8::Pythia pythia;
    Pythia8ToHepMC3 pythiaToHepMC;
    pythia.readString("Beams:eCM = 8000.");
    pythia.readString("HardQCD:all = on");
    pythia.readString("PhaseSpace:pTHatMin = 20.");
    pythia.init();
    for(int i = 0; i < 1000; ++i) 
    {
        if(!pythia.next()) continue;
        GenEvent* hepmc = new GenEvent(Units::GEV, Units::MM);
        pythiaToHepMC.fill_next_event(pythia.event, hepmc, -1, &pythia.info);
        w.writeEvent(hepmc);
    }
}


int main()
{
    LOG_DURATION("multithread");
    HepMCWriter w;
    std::vector<std::thread> threads;
    for(int i = 0; i < 7; ++i)
    {
        threads.emplace_back(generatorTread, std::ref(w));
    }
    for(auto& entry: threads)
    {
        entry.join();
    }
    return 0;
}
