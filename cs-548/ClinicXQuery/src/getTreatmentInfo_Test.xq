import module namespace lib = "http://www.lhu4.com/cs-548/xquery/clinic/lib" at "ClinicLib.xq";

import schema namespace clin = "http://www.lhu4.com/cs-548/schemas/clinic" at "Clinic.xsd";
import schema namespace ids = "http://www.lhu4.com/cs-548/schemas/clinic/clinic_ids" at "ClinicIds.xsd";
import schema namespace pat = "http://www.lhu4.com/cs-548/schemas/clinic/patient" at "Patient.xsd";


(: ****************getTreatmentInfo************************ :)

let $clinic := doc("ClinicData.xml")/clin:clinic
return lib:getTreatmentInfo($clinic)

