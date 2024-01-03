module namespace lib = "http://www.lhu4.com/cs-548/xquery/clinic/lib";

import schema namespace pat = "http://www.lhu4.com/cs-548/schemas/clinic/patient" at "Patient.xsd";
import schema namespace clin = "http://www.lhu4.com/cs-548/schemas/clinic" at "Clinic.xsd";
import schema namespace ids = "http://www.lhu4.com/cs-548/schemas/clinic/clinic_ids" at "ClinicIds.xsd";
import schema namespace trmt = "http://www.lhu4.com/cs-548/schemas/clinic/treatment" at "Treatment.xsd";
import schema namespace prov = "http://www.lhu4.com/cs-548/schemas/clinic/provider" at "Provider.xsd";

declare function lib:getPatientTreatments($patient as element(clin:patient), $clinic as element(clin:clinic))
    as element(lib:treatments) {

    <lib:treatments>
    {
        for $treatment in $patient/pat:treatments/pat:treatment
        return
        <lib:treatment>
            <lib:providerId> { $treatment/trmt:ProviderId/text() } </lib:providerId>
            <lib:diagnosis> { $treatment/trmt:diagnosis/text() } </lib:diagnosis>
        {
            typeswitch($treatment/child::*[last()])
            case $r as element(trmt:radiologyTreatment)
            return
            <lib:radiologyTreatment>
            {
                for $date in $r/trmt:date
                return
                <lib:date> { $date/text() } </lib:date>
            }
            </lib:radiologyTreatment>
            case $s as element(trmt:surgeryTreatment)
            return
            <lib:surgeryTreatment> 
                <lib:date> { $s/trmt:date/text() } </lib:date>
            </lib:surgeryTreatment>
            case $d as element(trmt:drugTreatment)
            return
            <lib:drugTreatment>
                <lib:drugName> { $d/trmt:drugName/text() } </lib:drugName>
                <lib:dosage> { $d/trmt:dosage/text() } </lib:dosage>
            </lib:drugTreatment>
            default
            return {} 
        }
        </lib:treatment>
    }
    </lib:treatments>
};

declare function lib:getPatientDrugs($patient as element(clin:patient), $clinic as element(clin:clinic))
    as element(lib:drugTreatments) {
    
    <lib:drugTreatments>
    {
        for $drugTreatment in $patient/pat:treatments/pat:treatment[trmt:drugTreatment]
        return  
            <lib:drugTreatment> 
                <lib:drugName> { $drugTreatment/trmt:drugTreatment/trmt:drugName/text() } </lib:drugName>
                <lib:diagnosis> { $drugTreatment/trmt:diagnosis/text() } </lib:diagnosis>
                <lib:dosage> { $drugTreatment/trmt:drugTreatment/trmt:dosage/text() } </lib:dosage>
            </lib:drugTreatment>
    }
    </lib:drugTreatments>
};

declare function lib:getTreatmentInfo($clinic as element(clin:clinic))
    as element(lib:treatments) {
    <lib:treatments>
    {
        for $patient in $clinic/clin:patient
        return
            for $treatment in $patient/pat:treatments/pat:treatment,
                $provider in $clinic/clin:provider
            where $treatment/trmt:ProviderId = $provider/prov:providerId
            return
            <lib:treatment>
                 <lib:patient>
                    <lib:patientId> { $patient/pat:patientId/text() } </lib:patientId>
                    <lib:patientName> { $patient/pat:name/text() } </lib:patientName>
                    <lib:birthDate> { $patient/pat:birthDate/text() } </lib:birthDate>
                  </lib:patient> 
                  <lib:provider>
                    <lib:providerId> { $provider/prov:providerId/text() } </lib:providerId>
                    <lib:name> { $provider/prov:name/text() } </lib:name>
                    <lib:specialization> { $provider/prov:specialization/text() } </lib:specialization>
                  </lib:provider>
                  <lib:diagnosis> { $treatment/trmt:diagnosis/text() } </lib:diagnosis>
                    {
                        typeswitch($treatment/child::*[last()])
                        case $r as element(trmt:radiologyTreatment)
                        return
                        <lib:radiologyTreatment>
                        {
                            for $date in $r/trmt:date
                            return
                            <lib:date> { $date/text() } </lib:date>
                        }
                        </lib:radiologyTreatment>
                        case $s as element(trmt:surgeryTreatment)
                        return
                        <lib:surgeryTreatment> 
                            <lib:date> { $s/trmt:date/text() } </lib:date>
                        </lib:surgeryTreatment>
                        case $d as element(trmt:drugTreatment)
                        return
                        <lib:drugTreatment>
                            <lib:drugName> { $d/trmt:drugName/text() } </lib:drugName>
                            <lib:dosage> { $d/trmt:dosage/text() } </lib:dosage>
                        </lib:drugTreatment>
                        default
                        return {} 
                    }
            </lib:treatment>
    }
    </lib:treatments>
};

declare function lib:getProviderInfo($clinic as element(clin:clinic))
    as element(lib:providers) {
    <lib:providers>
    {
        for $provider in $clinic/clin:provider
        return
        <lib:provider>
            <lib:providerId> { $provider/prov:providerId/text() } </lib:providerId>
            <lib:name> { $provider/prov:name/text() } </lib:name>
            <lib:specialization> { $provider/prov:specialization/text() } </lib:specialization>
            <lib:patients>
            {
                for $patient in $clinic/clin:patient
                where $patient//trmt:ProviderId = $provider/prov:providerId
                return
                <lib:patient>
                    <lib:patientId> { $patient/pat:patientId/text() } </lib:patientId>
                    <lib:patientName> { $patient/pat:name/text() } </lib:patientName>
                    <lib:birthDate> { $patient/pat:birthDate/text() } </lib:birthDate>
                    <lib:treatments>
                    {
                        for $treatment in $patient/pat:treatments/pat:treatment
                        where $treatment/trmt:ProviderId = $provider/prov:providerId
                        return
                        <lib:treatment>
                            <lib:diagnosis> { $treatment/trmt:diagnosis/text() } </lib:diagnosis>
                            {
                                typeswitch($treatment/child::*[last()])
                                case $r as element(trmt:radiologyTreatment)
                                return
                                <lib:radiologyTreatment>
                                {
                                    for $date in $r/trmt:date
                                    return
                                    <lib:date> { $date/text() } </lib:date>
                                }
                                </lib:radiologyTreatment>
                                case $s as element(trmt:surgeryTreatment)
                                return
                                <lib:surgeryTreatment> 
                                    <lib:date> { $s/trmt:date/text() } </lib:date>
                                </lib:surgeryTreatment>
                                case $d as element(trmt:drugTreatment)
                                return
                                <lib:drugTreatment>
                                    <lib:drugName> { $d/trmt:drugName/text() } </lib:drugName>
                                    <lib:dosage> { $d/trmt:dosage/text() } </lib:dosage>
                                </lib:drugTreatment>
                                default
                                return {} 
                            }
                        </lib:treatment>
                    }
                    </lib:treatments>
                </lib:patient>
            }
            </lib:patients>
        </lib:provider>
    }
    </lib:providers>
 };
 
declare function lib:getDrugInfo($clinic as element(clin:clinic))
    as element(lib:drugs) {
    <lib:drugs>
    {
        for $drug in distinct-values($clinic/clin:patient/pat:treatments/pat:treatment/trmt:drugTreatment/trmt:drugName)
        return
        <lib:drug>
            <lib:name> { $drug } </lib:name>
            <lib:patients>
            {
                for $patient in $clinic/clin:patient
                where $drug = $patient//trmt:drugName
                return
                <lib:patient>
                    <lib:patientId> { $patient/pat:patientId/text() } </lib:patientId>
                    <lib:patientName> { $patient/pat:name/text() } </lib:patientName>
                    <lib:birthDate> { $patient/pat:birthDate/text() } </lib:birthDate>
                    <lib:treatments>
                    {
                        for $drugTreatment in $patient/pat:treatments/pat:treatment,
                            $provider in $clinic/clin:provider
                        where $drug = $drugTreatment//trmt:drugName 
                            and $drugTreatment/trmt:ProviderId = $provider/prov:providerId
                        return
                        <lib:treatment>
                            <lib:diagnosis> { $drugTreatment/trmt:diagnosis/text() } </lib:diagnosis>
                            <lib:physicianId> { $provider/prov:providerId/text() } </lib:physicianId>
                            <lib:physicianName> { $provider/prov:name/text() } </lib:physicianName>
                        </lib:treatment>
                    }
                    </lib:treatments>
                </lib:patient>
            }
            </lib:patients>
        </lib:drug>
    }
    </lib:drugs>
};
 