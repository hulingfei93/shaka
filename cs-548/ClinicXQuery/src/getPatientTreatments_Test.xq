import module namespace lib = "http://www.lhu4.com/cs-548/xquery/clinic/lib" at "ClinicLib.xq";

import schema namespace clin = "http://www.lhu4.com/cs-548/schemas/clinic" at "Clinic.xsd";
import schema namespace ids = "http://www.lhu4.com/cs-548/schemas/clinic/clinic_ids" at "ClinicIds.xsd";
import schema namespace pat = "http://www.lhu4.com/cs-548/schemas/clinic/patient" at "Patient.xsd";



(: ****************getPatientTreatments************************ :)
let $clinic := doc("ClinicData.xml")/clin:clinic
let $patient := $clinic/clin:patient[pat:patientId = 357278]
return lib:getPatientTreatments($patient, $clinic)
