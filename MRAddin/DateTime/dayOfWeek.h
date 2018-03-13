//////////////////////////////////////////////////////////////////////////////////
#mkt int DoW(double date);
#imp
int Mkt::DoW(double date)
{
    return sim.DoW(date);
}
#
//////////////////////////////////////////////////////////////////////////////////

///
/// Return the day of the week for a given date:
///     0 : Monday
///     1 : Tuesday
///     2 : Wednesday
///     3 : Thursday
///     4 : Friday
///     5 : Saturday
///     6 : Sunday
///
/// (eg. 20060602 is a Friday, so returns 4)
/// Works for dates before 1 January 1970.
///
#sim int DoW(double date);
#imp
int Sim::DoW(double date)
{
    return (DateTime(static_cast<unsigned long>(date),0).dayOfWeek()+5) %7;
}
#