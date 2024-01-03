package edu.stevens.cs548.clinic.service.ejb;

import java.util.List;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import javax.ejb.Stateless;
import javax.inject.Inject;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;

import edu.stevens.cs548.clinic.domain.IPatientDAO;
import edu.stevens.cs548.clinic.domain.IPatientDAO.PatientException;
import edu.stevens.cs548.clinic.domain.IProviderDAO;
import edu.stevens.cs548.clinic.domain.IProviderDAO.ProviderException;
import edu.stevens.cs548.clinic.domain.IProviderFactory;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import edu.stevens.cs548.clinic.domain.Patient;
import edu.stevens.cs548.clinic.domain.PatientDAO;
import edu.stevens.cs548.clinic.domain.Provider;
import edu.stevens.cs548.clinic.domain.ProviderDAO;
import edu.stevens.cs548.clinic.domain.ProviderFactory;
import edu.stevens.cs548.clinic.domain.ProviderSpecEnum;
import edu.stevens.cs548.clinic.domain.Treatment;
import edu.stevens.cs548.clinic.domain.TreatmentDAO;
import edu.stevens.cs548.clinic.service.dto.DrugTreatmentType;
import edu.stevens.cs548.clinic.service.dto.ObjectFactory;
import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.dto.RadiologyTreatmentType;
import edu.stevens.cs548.clinic.service.dto.SurgeryTreatmentType;
import edu.stevens.cs548.clinic.service.dto.SpeicalizationType;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.ClinicDomainProducer.ClinicDomain;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientServiceException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderNotFoundException;


@Stateless(name="ProviderServiceBean")
public class ProviderService implements IProviderServiceLocal, IProviderServiceRemote {

    ObjectFactory DTOObjectFactory;
    private IProviderFactory providerFactory;
    private IPatientDAO patientDAO;
    private IProviderDAO providerDAO;
    private ITreatmentDAO treatmentDAO; 
    
     /**
     * Default constructor. 
     */
    public ProviderService() {
        providerFactory = new ProviderFactory();
        DTOObjectFactory = new ObjectFactory();
    }
    
    @PersistenceContext(unitName="ClinicDomain")
    private EntityManager em;
    
    @PostConstruct
    private void initialize() {
        patientDAO = new PatientDAO(em);
        providerDAO = new ProviderDAO(em);
        treatmentDAO = new TreatmentDAO(em);
    }
    
    private ProviderDTO convertProviderPDOToDTO(Provider provider) {
        ProviderDTO providerDTO = DTOObjectFactory.createProviderDTO();
        providerDTO.setNPI(provider.getNPI());
        providerDTO.setName(provider.getName());
        SpeicalizationType spec = SpeicalizationType.fromValue(provider.getSpecialization().toString());
        providerDTO.setSpecialization(spec);
        List<Long> tids = providerDTO.getTreatments();
        for(long tid : provider.getTreatmentIds()) {
            tids.add(tid);
        }
        return providerDTO;
    }
    
    @Override
    public long createProvider(String name, long NPI)
            throws ProviderServiceException {
        Provider provider = providerFactory.createProvider(NPI, name, ProviderSpecEnum.Unknown);
        try {
            providerDAO.addProvider(NPI, name, ProviderSpecEnum.Unknown);
        }
        catch(ProviderException ex) {
            throw new ProviderServiceException(ex.toString());
        }
        return provider.getNPI();
    }

    @Override
    public ProviderDTO[] getProviderByName(String name) {
        List<Provider> providers = providerDAO.getProviderByName(name);
        ProviderDTO[] providerDTOs = new ProviderDTO[providers.size()];
        for(int i = 0; i < providerDTOs.length; ++i) {
            providerDTOs[i] = convertProviderPDOToDTO(providers.get(i));
        }
        return providerDTOs;
    }

    @Override
    public ProviderDTO getProviderByNPI(long NPI)
            throws ProviderNotFoundException, ProviderServiceException {
        try {
            Provider provider = providerDAO.getProviderByNPI(NPI);
            if(provider == null)
                throw new ProviderNotFoundException("Provider not found NPI = " + NPI);
            return convertProviderPDOToDTO(provider);
        }
        catch(ProviderException ex) {
            throw new ProviderServiceException(ex.toString());
        }
    }

    @Override
    public long addTreatment(long patientDbId, long NPI, TreatmentDTO treatment)
            throws PatientNotFoundException, ProviderNotFoundException,
            ProviderServiceException {
        try {
                long result;
                Patient patient = patientDAO.getPatientByDbId(patientDbId);
                Provider provider = providerDAO.getProviderByNPI(NPI);
                if(provider == null)
                    throw new ProviderNotFoundException("Provider not found NPI = " + NPI);
                
                if(treatment.getDrugTreatment() != null) {
                    DrugTreatmentType drug = treatment.getDrugTreatment();
                    result = provider.addDrugTreatment(treatment.getDiagnosis(), drug.getDrugName(), drug.getDosage(), patient);
                }
                else if(treatment.getRadiologyTreatment() != null) {
                    RadiologyTreatmentType radiology = treatment.getRadiologyTreatment();
                    result = provider.addRadiologyTreatment(treatment.getDiagnosis(), radiology.getDate(), patient);
                }
                else {
                    SurgeryTreatmentType surgery = treatment.getSurgeryTreatment();
                    result = provider.addSurgeryTreatment(treatment.getDiagnosis(), surgery.getDate(), patient);
                }
                return result;
        }
        catch(PatientException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
        catch(ProviderException ex) {
            throw new ProviderServiceException(ex.toString());
        }
    }
        

    @Override
    public void deleteTreatment(long tid, long patientDbId, long NPI)
            throws TreatmentNotFoundException, ProviderServiceException {
        try {
            Treatment t = treatmentDAO.getTreatmentByDbId(tid);
            Patient patient = t.getPatient();
            Provider provider = t.getProvider();
            if(patient.getId() != patientDbId)
                throw new ProviderServiceException("Treatment with id " + tid + " is not owned by patient with id " + patientDbId);
            if(provider.getNPI() != NPI)
                throw new ProviderServiceException("Treatment with id " + tid + " is not supervised by provider with NPI " + NPI);
            treatmentDAO.deleteTreatment(t);
        }
        catch(TreatmentException ex) {
            throw new TreatmentNotFoundException(ex.toString());
        }
    }

    @Override
    public TreatmentDTO[] getTreatments(long patientDbId, long NPI) throws ProviderNotFoundException, ProviderServiceException {
        try {
            Provider provider = providerDAO.getProviderByNPI(NPI);
            Patient patient = patientDAO.getPatientByDbId(patientDbId);
            if(provider == null)
                throw new ProviderNotFoundException("Provider not found NPI = " + NPI);
            List<Long> tids = provider.getTreatmentIds(patient);
            TreatmentDTO[] treatmentDTOs = new TreatmentDTO[tids.size()];
            for(int i = 0; i < tids.size(); ++i) {
                TreatmentPDOToDTO visitor = new TreatmentPDOToDTO();
                provider.visitTreatment(tids.get(i), visitor);
                treatmentDTOs[i] = visitor.getTreatmentDTO();
            }
            return treatmentDTOs;
        }
        catch(PatientException ex) {
            throw new PatientNotFoundException(ex.toString());
        }
        catch(ProviderException ex) {
            throw new ProviderServiceException(ex.toString());
        }
        catch(TreatmentException ex) {
            throw new ProviderServiceException(ex.toString());
        }
    }

    @Override
    public TreatmentDTO[] getTreatments(long NPI)
            throws ProviderNotFoundException, ProviderServiceException {
        try {
            Provider provider = providerDAO.getProviderByNPI(NPI);
            if(provider == null)
                throw new ProviderNotFoundException("Provider not found NPI = " + NPI);
            List<Long> tids = provider.getTreatmentIds();
            TreatmentDTO[] treatmentDTOs = new TreatmentDTO[tids.size()];
            for(int i = 0; i < tids.size(); ++i) {
                TreatmentPDOToDTO visitor = new TreatmentPDOToDTO();
                provider.visitTreatment(tids.get(i), visitor);
                treatmentDTOs[i] = visitor.getTreatmentDTO();
            }
            return treatmentDTOs;
        }
        catch(ProviderException ex) {
            throw new ProviderServiceException(ex.toString());
        }
        catch(TreatmentException ex) {
            throw new ProviderServiceException(ex.toString());
        }
    }
}
