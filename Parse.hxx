#ifndef MSRP_GENERICPARSE_HXX
#define MSRP_GENERICPARSE_HXX

#include <sstream>
#include <stdexcept>

#include <boost/spirit.hpp>
#include <boost/spirit/actor.hpp>

#include <asio/buffer.hpp>

#include "ParseException.hxx"

namespace msrp
{

template<
   typename GrammarT,
   typename ResultT
   >
static inline void
Parse(ResultT& result, const asio::const_buffer& buf, const GrammarT& gr)
{
   using boost::spirit::parse_info;
   using boost::spirit::parse;

   try
   {
      parse_info<> info = parse(
            asio::buffer_cast<const char*>(buf),
            asio::buffer_cast<const char*>(buf) + buffer_size(buf),
            gr[boost::spirit::assign_a(result)]);

      if (!info.full)
      {
         std::stringstream ss;
         ss << "Parse failure (context: \"" << info.stop << "\")";

         throw ParseException(ss.str(), codeContext());
      }
   }
   catch (const ParseException& e)  // disambiguate from std::exception
   {
      throw e;
   }
   catch (const std::exception& e)
   {
      throw ParseException(e.what(), codeContext());
   }
}

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