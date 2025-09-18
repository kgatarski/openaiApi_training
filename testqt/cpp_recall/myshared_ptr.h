#include <iostream>
#include <mutex>
#include <atomic>

using namespace std;


template <typename T>
class myshared_ptr
{
    public:
        myshared_ptr() {
            cout << "ctor() 0x" << this << " " << use_count() << "\n";
        }

        myshared_ptr(T *t) {
            m_refcnt = new std::atomic<uint64_t>();
            m_ptr = t;
            m_refcnt->fetch_add(1, std::memory_order_relaxed);
            cout << "ctor(t *t) 0x" << this << " " << use_count() << "\n";
        }

        myshared_ptr(const myshared_ptr& copy) {
            m_refcnt = copy.m_refcnt;
            m_ptr = copy.m_ptr;
            if (m_refcnt)
                m_refcnt->fetch_add(1, std::memory_order_relaxed);
            cout << "copy ctor() 0x" << this << " from 0x" << &copy << " " << use_count() << "\n";
        }

        ~myshared_ptr() {
            cout << "dtor() " << "0x" << this << " " << use_count() << "\n";
            check_and_release();
        }

        // move ctor
        myshared_ptr(myshared_ptr&& movedObj) {
            m_refcnt = movedObj.m_refcnt;
            m_ptr = movedObj.m_ptr;
            movedObj.m_refcnt = nullptr;
            movedObj.m_ptr = nullptr;
            cout << "move ctor(&&) 0x" << this << " from 0x" << &movedObj << " " << use_count() << "\n";
        }

        //move assignment operator
        myshared_ptr& operator=(myshared_ptr&& other) {
            cout << "move operator= this=0x" << this << " other=0x" << &other << "\n";
            if (this != &other) {
                check_and_release();                
                m_refcnt = other.m_refcnt;
                m_ptr = other.m_ptr;
                other.m_refcnt = nullptr;
                other.m_ptr = nullptr;
            }
            cout << "op+(&&) " << use_count() << "\n";
            return *this;
        }

        T& operator*() {
            if (!m_ptr) throw std::runtime_error("Dereferencing null pointer");
            return *m_ptr;
        }

        T* operator->() {
            if (!m_ptr) throw std::runtime_error("Dereferencing null pointer");
            return m_ptr;
        }

        // copy assignment
        myshared_ptr& operator=(const myshared_ptr& other) {
            cout << "copy operator= this=0x" << this << " other=0x" << &other << "\n";
            if (this != &other) {
                check_and_release();                
                m_refcnt = other.m_refcnt;
                m_ptr = other.m_ptr;
                if (m_refcnt)
                    m_refcnt->fetch_add(1, std::memory_order_relaxed);
            }
            cout << "opertor=() " << use_count() << endl;
            return *this;
        }

        void reset() {
            check_and_release();
            m_refcnt = nullptr;
            m_ptr = nullptr;
        }

        T* get() {
            return m_ptr;
        }

        uint64_t use_count() {
            return ((nullptr != m_refcnt) ? m_refcnt->load(std::memory_order_relaxed) : 0);
        }

    private:
        void check_and_release() {
            if ((nullptr != m_refcnt) && (m_refcnt->fetch_sub(1, std::memory_order_relaxed) == 1)) {
                cout << "  releasing resource \n";
                delete m_ptr;
                delete m_refcnt;
            }
        }
        std::atomic<uint64_t> *m_refcnt = nullptr;
        T *m_ptr = nullptr;
};