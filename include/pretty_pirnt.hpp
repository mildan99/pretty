

#ifndef MEMES_HPP
#define MEMES_HPP 1 

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <type_traits>
#include <iterator>
#include <string_view>
#include <tuple>



namespace memes
{
    namespace functional
    {

        template <class F>
        struct function_traits;

        // function pointer
        template <class R, class... Args>
        struct function_traits<R (*)(Args...)> : public function_traits<R(Args...)>
        {
        };

        template <class R, class... Args>
        struct function_traits<R(Args...)>
        {
            using return_type = R;

            static constexpr std::size_t arity = sizeof...(Args);
            using args_tuple = std::tuple<Args...>;
        };
        template <class C, class R, class... Args>
        struct function_traits<R (C::*)(Args...)> : public function_traits<R(C &, Args...)>
        {
        };

        // const member function pointer
        template <class C, class R, class... Args>
        struct function_traits<R (C::*)(Args...) const> : public function_traits<R(C &, Args...)>
        {
        };

        // member object pointer
        template <class C, class R>
        struct function_traits<R(C::*)> : public function_traits<R(C &)>
        {
        };

        template <class F>
        struct function_traits
        {
        private:
            using call_type = function_traits<decltype(&F::operator())>;

            template <class C>
            struct args_rest
            {
            };

            template <class First, class... Rest>
            struct args_rest<std::tuple<First, Rest...>>
            {
                using args_type = std::tuple<Rest...>;
            };

        public:
            using return_type = typename call_type::return_type;
            using args_tuple = typename args_rest<typename call_type::args_tuple>::args_type;

            static constexpr std::size_t arity = call_type::arity - 1;
        };

        template <class F>
        struct function_traits<F &> : public function_traits<F>
        {
        };

        template <class F>
        struct function_traits<F &&> : public function_traits<F>
        {
        };
    } // namespace functional

    template <class ContainerType>
    concept Container = requires(ContainerType a, const ContainerType b)
    {
        requires std::regular<ContainerType>;
        requires std::swappable<ContainerType>;
        requires std::destructible<typename ContainerType::value_type>;
        requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
        requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
        requires std::forward_iterator<typename ContainerType::iterator>;
        requires std::forward_iterator<typename ContainerType::const_iterator>;
        requires std::signed_integral<typename ContainerType::difference_type>;
        requires std::same_as<typename ContainerType::difference_type,
                              typename std::iterator_traits<typename ContainerType::iterator>::difference_type>;
        requires std::same_as<typename ContainerType::difference_type,
                              typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
        {a.begin()      } -> std::same_as<typename ContainerType::iterator>;
        {a.end()        } -> std::same_as<typename ContainerType::iterator>;
        {b.begin()      } -> std::same_as<typename ContainerType::const_iterator>;
        {b.end()        } -> std::same_as<typename ContainerType::const_iterator>;
        {a.cbegin()     } -> std::same_as<typename ContainerType::const_iterator>;
        {a.cend()       } -> std::same_as<typename ContainerType::const_iterator>;
        {a.size()       } -> std::same_as<typename ContainerType::size_type>;
        {a.max_size()   } -> std::same_as<typename ContainerType::size_type>;
        {a.empty()      } -> std::same_as<bool>;
    };

        template <class First, class... Args>
        inline void print(First&& f,Args && ...a);
        template <class First, class... Args>
        inline void println(First&& f,Args && ...a);
    namespace print_impl
    {
 

        template <class T>
        inline  void do_print( T && elem) { fmt::print("{}",elem); }

        template<class T1,class T2>
        inline void do_print( std::pair<T1,T2> & );
        template<class T1,class T2>
        inline void do_print( std::pair<T1,T2> && );
        template<class... T>
        inline void do_print( std::tuple<T...> & );
        template<class... T>
        inline void do_print( std::tuple<T...> && );

        template <class T, size_t N>
        inline auto do_print(T(&arr)[N])
        {
            for (size_t i = 0; i < N; i++)
            {
                do_print(arr[i]);
                if (i+1<N)   fmt::print(",");                
            }
        }
        
        template <Container T>
        inline auto do_print(T & v)
        {
            fmt::print("[");
            for (auto it = v.begin(); it != v.end();std::advance(it,1))
            {
                do_print(*it);
                if(std::next(it)!=v.end())
                    fmt::print(",");
            };
            fmt::print("]");
        }


        template <Container T>
        inline auto do_print(T && v)
        {
            fmt::print("[");
            for (auto it = v.begin(); it != v.end();std::advance(it,1))
            {
                do_print(*it);
                if(std::next(it)!=v.end())
                    fmt::print(",");
            };
            fmt::print("]");
        }

        
        template <>
        inline auto do_print<>( std::string_view & sv)
        {
            fmt::print("\"{}\"",sv);
        }

        template <>
        inline auto do_print<>( std::string_view && sv)
        {
            fmt::print("\"{}\"",sv);
        }

        template <>
        inline auto do_print<>( std::string & s)
        {
            fmt::print("\"{}\"",s);
        }
        template <>
        inline auto do_print<>( std::string && s)
        {
            fmt::print("\"{}\"",s);
        }

        inline auto do_print(const char *str)
        {
            do_print(std::string_view{str});
        }





        template<class... T>
        inline void do_print( std::tuple<T...>& t){
            std::apply([](auto&&... args) {
            fmt::print("[");
                             print(std::forward<decltype(args)>(args)...);
            fmt::print("]");
                              },
                      t);

        }
        template<class... T>
        inline void do_print( std::tuple<T...>&& t){
            std::apply([](auto&&... args) {
            fmt::print("[");
                             print(std::forward<decltype(args)>(args)...);
            fmt::print("]");
                              },
                      t);

        }
        
        template<class T1,class T2>
        inline void do_print( std::pair<T1,T2>& t){     
            std::apply([](auto&&... args) {
            fmt::print("[");
                             memes::print(std::forward<decltype(args)>(args)...);
            fmt::print("]");
                              },
                      t);

        }

        template<class T1,class T2>
        inline void do_print( std::pair<T1,T2>&& t){
            std::apply([](auto&&... args) {
            fmt::print("[");
                             print(std::forward<decltype(args)>(args)...); 
            fmt::print("]");
                             },
                      t);

        }

       
    } // namespace print

        template <class First, class... Args>
        inline void print(First&& f,Args && ...a)
        {
            ((print_impl::do_print(std::forward<First>(f))));
            (((fmt::print(",") ,print_impl::do_print(std::forward<Args>(a)) ), ...));
        }


        
        template <class First, class... Args>
        inline void println(First&& f,Args && ...a)
        {
            ((print_impl::do_print(std::forward<First>(f))));
            (((fmt::print(",") ,print_impl::do_print(std::forward<Args>(a)) ), ...));
            fmt::print("\n");
        }
}

#endif // !1
