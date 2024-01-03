package edu.stevens.cs548.clinic.service.web.soap;

import javax.ejb.EJB;
import javax.inject.Inject;
import javax.jws.WebService;

import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.IProviderServiceRemote;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderServiceException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.TreatmentNotFoundException;


@WebService(
        endpointInterface="edu.stevens.cs548.clinic.service.web.soap.IProviderWebService",
        targetNamespace="http://cs548.stevens.edu/clinic/service/web/soap/provider",
        serviceName="ProviderWebService",
        portName="ProviderWebPort")
public class ProviderWebService implements IProviderWebService {

    //@Inject
    @EJB
    private IProviderServiceRemote providerService;
    
    @Override
    public long createProvider(String name, long NPI)
            throws ProviderServiceException {
        return providerService.createProvider(name, NPI);
    }

    @Override
    public ProviderDTO[] getProviderByName(String name) {
        return providerService.getProviderByName(name);
    }

    @Override
    public ProviderDTO getProviderByNPI(long NPI)
            throws ProviderNotFoundException, ProviderServiceException {
        return providerService.getProviderByNPI(NPI);
    }

    @Override
    public long addTreatment(long patientDbId, long NPI, TreatmentDTO treatment)
            throws PatientNotFoundException, ProviderNotFoundException,
            ProviderServiceException {
        return providerService.addTreatment(patientDbId, NPI, treatment);
    }

    @Override
    public void deleteTreatment(long tid, long patientDbId, long NPI)
            throws TreatmentNotFoundException, ProviderServiceException {
        providerService.deleteTreatment(tid, patientDbId, NPI);
    }

    @Override
    public TreatmentDTO[] getTreatmentsBy_PatientDbId_NPI(long patientDbId, long NPI)
            throws ProviderNotFoundException, ProviderServiceException {
        return providerService.getTreatments(patientDbId, NPI);
    }

    @Override
    public TreatmentDTO[] getTreatments(long NPI)
            throws ProviderNotFoundException, ProviderServiceException {
        return providerService.getTreatments(NPI);
    }
}
