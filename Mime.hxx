#ifndef MSRP_MIME_HXX
#define MSRP_MIME_HXX

#include <ostream>
#include <map>
#include <string>

#include <boost/mem_fn.hpp>
#include <boost/spirit.hpp>
#include <boost/spirit/actor.hpp>
#include <boost/spirit/attribute/closure.hpp>
#include <boost/spirit/phoenix.hpp>

namespace msrp
{

class Mime
{
   public:
      Mime()
      {}

      Mime(const std::string& type, const std::string& subtype) :
         mType(type), mSubtype(subtype)
      {}

      Mime(const std::pair<std::string, std::string>& pair) :
         mType(pair.first), mSubtype(pair.second)
      {}

      const std::string& type() const { return mType; }
      std::string& type() { return mType; }

      const std::string& subtype() const { return mSubtype; }
      std::string& subtype() { return mSubtype; }

      const std::map<std::string, std::string>& params() const { return mParams; }
      std::map<std::string, std::string>& params() { return mParams; }

      bool operator<(const Mime&) const;

      bool operator==(const Mime&) const;

   private:
      std::string mType;
      std::string mSubtype;

      std::map<std::string, std::string> mParams;
};

std::ostream&
operator<<(std::ostream&, const Mime&);

namespace parser
{

struct MimeClosure : boost::spirit::closure<MimeClosure, msrp::Mime>
{
   member1 mime;
};

struct Mime : boost::spirit::grammar<Mime, MimeClosure::context_t>
{
   template<typename ScannerT>
   struct definition
   {
      definition(const Mime& self)
      {
         using phoenix::bind;
         using phoenix::construct_;

         root = type[bind(boost::mem_fn<std::string&>(&msrp::Mime::type))(self.mime) =
                  construct_<std::string>(phoenix::arg1, phoenix::arg2)]
            >> '/'
            >> type[bind(boost::mem_fn<std::string&>(&msrp::Mime::subtype))(self.mime) =
                  construct_<std::string>(phoenix::arg1, phoenix::arg2)]
            >> *param
            ;

         type = +boost::spirit::alnum_p | boost::spirit::ch_p('*');

         param = boost::spirit::ch_p(';')
            >> (+boost::spirit::alnum_p)
                  [phoenix::var(key) = phoenix::construct_<std::string>(phoenix::arg1, phoenix::arg2)]
            >> '='
            >> (
                 (boost::spirit::ch_p('"')
                  >> (*(
                        boost::spirit::anychar_p - boost::spirit::ch_p('"')
                       )
                     )[boost::spirit::insert_at_a(
                          bind(boost::mem_fn<ParamMap&>(&msrp::Mime::params))(self.mime)(),
                          key)]
                  >> boost::spirit::ch_p('"')
                  )
               | (*(boost::spirit::anychar_p
                       - boost::spirit::blank_p
                       - boost::spirit::ch_p(';'))
                 )[boost::spirit::insert_at_a(
                       bind(boost::mem_fn<ParamMap&>(&msrp::Mime::params))(self.mime)(),
                       key)]
               )
            ;
      }

      std::string key;

      boost::spirit::rule<ScannerT> root;
      boost::spirit::rule<ScannerT> type;
      boost::spirit::rule<ScannerT> param;

      boost::spirit::rule<ScannerT> const& start() const
      {
         return root;
      }
   };

   typedef std::map<std::string, std::string> ParamMap;
};

} // namespace parser

}

#endif

// Copyright 2007 Chris Bond
// 
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
